require_relative 'Debugger'
require_relative 'Console'
require_relative 'CallStack'
require_relative 'Threads'
require_relative 'Breakpoints'

module App

  class Application
    def initialize(debug)
      @editors = { }
      @activeEditors = [ ]

      @application = UI::Application.new()
      @mainwindow = UI::MainWindow.new()
      @debugger = App::Debugger.new(@mainwindow)

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
        if (@mainwindow.process.currentState == LldbDriver::Process::State[:Stopped])
          @mainwindow.process.continueExecution()
        else
          @mainwindow.process.pauseExecution()
        end
      })
      @processToolbar.hide()


      @callStack = App::CallStack.new(@mainwindow, @debugger)
      @threads = App::Threads.new(@mainwindow, @debugger)
      @breakpoints = App::Breakpoints.new(@mainwindow, @debugger)
      @console = App::Console.new(@mainwindow)
      @processWindows = [ @threads, @callStack ]

      processChanged(nil)
      @mainwindow.processChanged.listen { |p| processChanged(p) }

      @debugger.ready.listen do |process|
        loadCurrentSource()
        highlightSources()
      end

      @mainwindow.editorOpened.listen do |editor|
        if (editor.class == UI::FileEditor)
          setupFileEditor(editor)
        end
      end

      @mainwindow.targetChanged.listen do |t|
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
      end
      @debugger.ready.listen do |p|
        @actions[:pause_continue].setText("Continue")
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
      
      @mainwindow.process.threads.each do |t|
        frame = t.selectedFrame

        if (frame.hasLineNumber)
          current = t.isCurrent

          line = frame.lineNumber
          file =  frame.filename

          editor = @editors[file]
          @activeEditors << editor
          if (editor)
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