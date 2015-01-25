require_relative 'TextWindow'

module App

  class Values
    include TextWindow

    attr_reader :widget
    
    def initialize(mainWindow, debugger)
      @widget = mainWindow.addEditor("Values")

      @widget.setContents("test")
    end
  end
end