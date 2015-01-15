require_relative 'DebuggerTerminal'
require_relative 'Debugger'
require_relative 'Console'
require_relative 'CallStack'
require_relative 'Threads'
require_relative 'Breakpoints'

module App

  class Application
    def initialize()
      @editors = { }

      @application = UI::Application.new()
      @mainwindow = UI::MainWindow.new()
      @debugTerminal = App::DebuggerTerminal.new(@mainwindow)

      @debugger = App::Debugger.new(@mainwindow)

      @callStack = App::CallStack.new(@mainwindow, @debugger)
      @threads = App::Threads.new(@mainwindow, @debugger)
      @breakpoints = App::Breakpoints.new(@mainwindow, @debugger)
      @console = App::Console.new(@mainwindow)
      @processWindows = [ @threads, @callStack ]

      processChanged(nil)
      @mainwindow.processChanged.listen { |p| processChanged(p) }

      @debugger.ready.listen do |process|
        loadCurrentSource()
      end

      @mainwindow.editorOpened.listen do |editor|
        if (editor.class == UI::FileEditor)
          setupFileEditor(editor)
        end
      end

      @mainwindow.targetChanged.listen do |t|
        if (t != nil)
          t.breakpointsChanged.listen { |_| syncEditorBreakpoints() }
        end
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