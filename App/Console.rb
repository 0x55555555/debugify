
module App

  class Console

    attr_reader :widget

    def initialize(mainWindow, debugger)
      @widget = mainWindow.addConsole("Console")

      mainWindow.output.listen do |p|
        @widget.append(p)
      end

      mainWindow.errors.listen do |p|
        @widget.append(p)
      end

      debugger.processBegin.listen do |p|
        if (p != nil)
          @widget.clear()
        end
      end
    end

    def append(s)
      @widget.puts(s)
    end
  end

end