
module App

  class CallStack

    attr_reader :widget
    
    def initialize(mainWindow)
      @widget = mainWindow.addEditor("Call Stack")

      @widget.setContents("<a href=\"plink\">Plink</a><br><a href=\"plonk\">Plonk</a>")

      @widget.clicked.listen do |p|
        puts p
      end
    end
  end

end