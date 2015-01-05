
module App

  class CallStack
    def initialize(mainWindow)
      @editor = mainWindow.addEditor()
    end
  end

end