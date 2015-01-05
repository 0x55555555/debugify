
module App

  class Console
    def initialize(mainWindow)
      @console = mainWindow.addConsole("Console")
      mainWindow.output.listen do |p|
        @console.append(p)
      end

      mainWindow.errors.listen do |p|
        @console.append(p)
      end

      mainWindow.processStateChanged.listen do |p|
        @console.puts("Process state changed to #{p}")
      end
    end
  end

end