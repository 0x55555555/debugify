require_relative 'DebuggerTerminal'
require_relative 'Console'
require_relative 'CallStack'

module App

  class Application
    def initialize()
      @application = UI::Application.new()
      @mainwindow = UI::MainWindow.new()
      @debugTerminal = App::DebuggerTerminal.new(@mainwindow)

      @callStack = App::CallStack.new(@mainwindow)
      @console = App::Console.new(@mainwindow)
      @processWindows = [ @callStack, @console ]

      @mainwindow.targetChanged.listen do |t|
        puts "target now #{t}"
      end

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
          mainWindow.showDock(@console)
        else
          mainWindow.hideDock(@console)
        end
      end
    end
  end
end