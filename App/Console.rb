
module App

  class Console

    attr_reader :widget

    def initialize(mainWindow, debugger)
      @widget = mainWindow.addConsole("Console", false)

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