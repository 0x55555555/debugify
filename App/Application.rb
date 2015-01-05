require_relative 'DebuggerTerminal'

module App

  class Application
    def initialize()
      @application = UI::Application.new()
      @mainwindow = UI::MainWindow.new()
      @debugTerminal = App::DebuggerTerminal.new(@mainwindow.debuggerTerminal)

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