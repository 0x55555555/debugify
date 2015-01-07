require_relative 'DebuggerTerminal'
require_relative 'Debugger'
require_relative 'Console'
require_relative 'CallStack'
require_relative 'Threads'

module App

  class Application
    def initialize()
      @application = UI::Application.new()
      @mainwindow = UI::MainWindow.new()
      @debugTerminal = App::DebuggerTerminal.new(@mainwindow)

      @debugger = App::Debugger.new(@mainwindow)

      @callStack = App::CallStack.new(@mainwindow, @debugger)
      @threads = App::Threads.new(@mainwindow, @debugger)
      @console = App::Console.new(@mainwindow)
      @processWindows = [ @threads, @callStack ]

      processChanged(nil)
      @mainwindow.processChanged.listen { |p| processChanged(p) }
    end

    def execute()
      @mainwindow.show()
      @application.execute()
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