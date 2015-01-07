
module App

  class Console

    attr_reader :widget

    def initialize(mainWindow)
      @widget = mainWindow.addConsole("Console")

      mainWindow.output.listen do |p|
        @widget.append(p)
      end

      mainWindow.errors.listen do |p|
        @widget.append(p)
      end

      mainWindow.processStateChanged.listen do |p|
        @widget.puts("Process state changed to #{LldbDriver::Process::State[p]}")
      end

      mainWindow.processChanged.listen do |p|
        if (p != nil)
          @widget.clear()
        end
      end
    end
  end

end