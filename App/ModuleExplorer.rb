
module App

  class ModuleExplorer

    attr_reader :widget
    
    def initialize(mainWindow, debugger)
      @widget = mainWindow.addModuleExplorer("Modules", false)

      debugger.targetChanged.listen do |t|
        @widget.setTarget(t)
      end
    end

  end

end