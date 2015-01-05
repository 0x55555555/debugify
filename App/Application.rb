require_relative 'DebuggerTerminal'
require_relative 'Console'

module App

  class Application
    def initialize()
      @application = UI::Application.new()
      @mainwindow = UI::MainWindow.new()
      @debugTerminal = App::DebuggerTerminal.new(@mainwindow)
      @console = App::Console.new(@mainwindow)

      @mainwindow.targetChanged.listen do |t|
        puts "target now #{t}"
      end

      @mainwindow.processChanged.listen do |p|
        puts "process now #{p}"
      end
    end

    def execute()
      @mainwindow.show()
      @application.execute()
    end
  end
end