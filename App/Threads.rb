
module App

  class Threads

    attr_reader :widget
    
    def initialize(mainWindow)
      @widget = mainWindow.addEditor("Threads")

      

      @widget.clicked.listen do |p|
        puts p
      end

      mainWindow.processStateChanged.listen do |p|
        if (mainWindow.process)
          puts mainWindow.process.threads
          @widget.setContents("Threads: #{mainWindow.process.threads}")
        end
      end
    end
  end

end