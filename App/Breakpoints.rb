
module App

  class Breakpoints
    include TextWindow

    attr_reader :widget

    def initialize(mainWindow, debugger, project)
      @widget = mainWindow.addEditor("Breakpoints")

      project.install_handler(:breakpoints, self)

      @mainwindow = mainWindow

      @mainwindow.targetChanged.listen do |t|
        if (t)
          t.breakpointsChanged.listen { |_| updateBreakpoints() }
        end
      end

      @widget.clicked.listen do |f|
        target = @mainwindow.target
        if (target != nil)
          target.breakpoints.each do |b|
            if (b.id == f.to_i && b.locations.length)
              loc = b.locations[0]
              @mainwindow.openFile(loc.file, loc.disline)
            end
          end
        end
      end

      updateBreakpoints()
    end

    def updateBreakpoints()
      target = @mainwindow.target
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
          @mainwindow.target.addBreakpoint(b["file"], b["line"])
        end
      end
    end

    def serialise(handler)
      brks = []
      target = @mainwindow.target
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

      if (handler.has_location(:exe) && brks.length)
        handler.set_value(:breakpoints, brks, :exe)
      end
    end
  end

end