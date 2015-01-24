require_relative 'Debugger'
require_relative 'Console'
require_relative 'CallStack'
require_relative 'Threads'
require_relative 'Breakpoints'
require_relative 'Project'

module App

  class Logger

    attr_writer :console

    def log(s)
      @console.append(s)
    end
  end

  class Application
    def initialize(debug)
      @project = DebuggerProject.new(nil)
      @editors = { }
      @activeEditors = [ ]

      @log = Logger.new

      @application = UI::Application.new()
      @mainwindow = UI::MainWindow.new()
      @mainwindow.aboutToClose.listen do
        @project.close()
      end

      @debugger = App::Debugger.new(@mainwindow, @log)

      if (debug)
        require_relative 'DebuggerTerminal'
        @debugTerminal = App::DebuggerTerminal.new(@mainwindow)
      end

      @targetToolbar = @mainwindow.addToolBar("Target")
      @targetToolbar.addAction("Run", Proc.new {
        @debugger.launch()
      })
      @targetToolbar.hide()

      @processToolbar = @mainwindow.addToolBar("Process")
      @processToolbar.addAction("Kill", Proc.new {
        @mainwindow.process.kill()
      })
      @actions = { }
      @actions[:pause_continue] = @processToolbar.addAction("", Proc.new {
        if (@mainwindow.process.currentState == LldbDriver::ProcessState[:Stopped])
          @mainwindow.process.continueExecution()
        else
          @mainwindow.process.pauseExecution()
        end
      })
      @processToolbar.hide()

      @currentThreadToolbar = @mainwindow.addToolBar("Current Thread")
      @currentThreadToolbar.addAction("Step Into", Proc.new {
        @mainwindow.process.selectedThread.stepInto()
      })
      @currentThreadToolbar.addAction("Step Over", Proc.new {
        @mainwindow.process.selectedThread.stepOver()
      })
      @currentThreadToolbar.addAction("Step Out", Proc.new {
        @mainwindow.process.selectedThread.stepOut()
      })
      @currentThreadToolbar.hide()


      @callStack = App::CallStack.new(@mainwindow, @debugger)
      @threads = App::Threads.new(@mainwindow, @debugger)
      @breakpoints = App::Breakpoints.new(@mainwindow, @debugger, @project)
      @console = App::Console.new(@mainwindow)
      @log.console = @console
      @processWindows = [ @threads, @callStack ]

      processChanged(nil)
      @mainwindow.processChanged.listen { |p| processChanged(p) }

      @debugger.ready.listen do |process|
        loadCurrentSource()
        highlightSources()
        @currentThreadToolbar.show()
      end

      @mainwindow.editorOpened.listen do |editor|
        if (editor.class == UI::FileEditor)
          setupFileEditor(editor)
        end
      end

      @mainwindow.targetChanged.listen do |t|
        @project.reset(t.path)
        if (t != nil)
          t.breakpointsChanged.listen { |_| syncEditorBreakpoints() }
          @targetToolbar.show()
        else
          @targetToolbar.hide()
        end
      end

      @mainwindow.processChanged.listen do |p|
        if (p != nil)
          @processToolbar.show()
        else
          @processToolbar.hide()
        end
      end

      @debugger.running.listen do |p|
        @actions[:pause_continue].setText("Pause")
        clearSourceHighlight()
        @currentThreadToolbar.hide()
        @log.log "Process running"
      end
      @debugger.ready.listen do |p|
        @actions[:pause_continue].setText("Continue")
        @log.log "Process ready to debug"
      end
      @debugger.exited.listen do |p|
        @log.log "Process exited with code #{p.exitStatus}"
        desc = p.exitDescription
        if (desc)
          @log.log "  " + desc
        end
        @debugger.end()
      end
    end

    def execute()
      @mainwindow.show()
      @application.execute()
    end

  private
    def setupFileEditor(editor)
      path = editor.path()
      editor.marginClicked.listen do |line|
        found, brk, loc = @mainwindow.target.findBreakpoint(path, line)
        if (found)
          @mainwindow.target.removeBreakpoint(brk)
        else
          @mainwindow.target.addBreakpoint(path, line)
        end
      end

      @editors[path] = editor
      highlightSources()
      syncEditorBreakpoints()
    end

    def syncEditorBreakpoints()
      @editors.each { |_, e| e.clearMarkers(UI::FileEditor::MarkerType[:Breakpoint]) }
      target = @mainwindow.target
      target.breakpoints.each do |br|
        br.locations.each do |l|
          addEditorBreakpoint(l.file, l.line)
        end
      end
    end

    def addEditorBreakpoint(file, line)
      editor = @editors[file]

      editor.addMarker(UI::FileEditor::MarkerType[:Breakpoint], line) if editor
    end

    def loadCurrentSource()
      thread = @mainwindow.process.selectedThread
      frame = thread.selectedFrame

      file = frame.filename
      line = frame.lineNumber
      if (file.length > 0)
        editor = @mainwindow.openFile(file, line)
      end
    end

    def clearSourceHighlight()
      @activeEditors.each do |e|
        e.clearMarkers(UI::FileEditor::MarkerType[:ActiveLine])
        e.clearMarkers(UI::FileEditor::MarkerType[:CurrentLine])
      end
      @activeEditors.clear()
    end

    def highlightSources()
      clearSourceHighlight()

      if (@mainwindow.process == nil)
        return
      end
      
      @mainwindow.process.threads.each do |t|
        frame = t.selectedFrame

        if (frame.hasLineNumber)
          current = t.isCurrent

          line = frame.lineNumber
          file =  frame.filename

          editor = @editors[file]
          if (editor)
            @activeEditors << editor
            type = current ? UI::FileEditor::MarkerType[:CurrentLine] :
              UI::FileEditor::MarkerType[:ActiveLine]
            editor.addMarker(type, line)
          end
        end
      end
    end

    def processChanged(p)
      @processWindows.each do |w|
        if (p != nil)
          @mainwindow.showDock(w.widget)
        else
          @mainwindow.hideDock(w.widget)
        end
      end
    end
  end
end