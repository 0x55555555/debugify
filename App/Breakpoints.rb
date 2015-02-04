
module App

  class BreakpointEditor
    def self.show(target, oldId = nil)
      oldBr = nil
      oldLoc = nil
      target.breakpoints.each do |b|
        if (b.id == oldId)
          oldBr = b
          locs = oldBr.locations
          if (locs.length > 0)
            oldLoc = locs[0]
          end
        end
      end

      dlg = UI::Dialog.new(UI::UIC_PATH + "addbreakpoint.ui")

      if (oldLoc)
        dlg.setValue("file", oldLoc.file)
        dlg.setValue("line", oldLoc.line)
      end

      dlg.exec()
      if (UI::Dialog::Result[dlg.result()] == :Accepted)
        target.addBreakpoint(dlg.value("file"), dlg.value("line").to_i)

        if (oldBr != nil)
          target.removeBreakpoint(oldBr)
        end
      end
    end
  end

  class Breakpoints
    include TextWindow

    attr_reader :widget

    def initialize(mainWindow, debugger, project)
      @debugger = debugger
      @widget = mainWindow.addEditor("Breakpoints", true  )
      @mainwindow = mainWindow

      @widget.toolBar.addAction("Add", Proc.new { BreakpointEditor.show(@debugger.target) })

      project.install_handler(:breakpoints, self)

      @debugger.targetChanged.listen do |t|
        if (t)
          t.breakpointsChanged.listen { |_| updateBreakpoints() }
        end
      end

      @widget.clicked.listen do |f|
        target = @debugger.target
        if (target != nil)
          target.breakpoints.each do |b|
            if (b.id == f.to_i && b.locations.length > 0)
              loc = b.locations[0]
              @mainwindow.openFile(loc.file, loc.line)
            end
          end
        end
      end

      @widget.contextMenu.listen do |anch, x, y|
        if (anch.length != 0)
          men = UI::Menu.new(@widget)

          men.addAction("Edit", Proc.new { BreakpointEditor.show(@debugger.target, anch.to_i) })
          men.exec()
        end
      end

      updateBreakpoints()
    end

    def updateBreakpoints()
      target = @debugger.target
      if (target == nil)
        @widget.setContents("")
        return
      end

      breakpoints = target.breakpoints.map { |b| formatBreakpoint(b) }

      @widget.setContents(alternatingColourList(breakpoints))
    end

    def formatBreakpoint(b)
      loc = "#{b.id} "

      b.locations.each do |l|
        file = File.basename(l.file)
        loc << "#{file} #{l.line}"
      end

      col = b.enabled ? :black : :darkgray
      link(b.id, colour(loc, col))
    end

    # Methods for setting serialisation
    def owns_value(key)
      return key == :breakpoints
    end

    def deserialise(handler)
      if (handler.has_value(:breakpoints))
        brks = handler.value(:breakpoints)
        brks.each do |b|
          if (b.include?("file") && b.include?("line"))
            @debugger.target.addBreakpoint(b["file"], b["line"])
          end
        end
      end
    end

    def serialise(handler)
      brks = []
      target = @debugger.target
      if (target != nil)
        brk = { }
        brks << brk
        target.breakpoints.each do |b|
          if (b.locations.length)
            l = b.locations[0]
            brk["file"] = l.file
            brk["line"] = l.line
          end
        end
      end

      if (handler.has_location(:exe))
        handler.set_value(:breakpoints, brks, :exe)
      end
    end
  end

end