
module App

  class ModuleExplorer

    attr_reader :widget
    
    def initialize(mainWindow, debugger)
      @widget = mainWindow.addModuleExplorer("Modules", true)

      toolbar = @widget.toolBar

      reload = toolbar.addAction("Reload", Proc.new {
        if (debugger.target)
          debugger.load(debugger.target.path)
        end
      })

      addMod = toolbar.addAction("Add Module", Proc.new {
        mod = mainWindow.getOpenFilename("Add Module")  
        if (mod == "")
          next
        end

        debugger.addModule(mod)
      })

      acts = [ reload, addMod ]

      debugger.targetChanged.listen do |t|
        refreshModules(t, acts)
      end
    end

    def refreshModules(target, acts)
      puts "Refresh modules"
      @widget.setTarget(target)
      acts.each{ |a| a.setEnabled(target != nil) }
    end

  end

end