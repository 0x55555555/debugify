
module App

  class Breakpoints
    include TextWindow

    attr_reader :widget

    def initialize(mainWindow, debugger)
      @widget = mainWindow.addEditor("Breakpoints")

      @mainwindow = mainWindow

      @mainwindow.targetChanged.listen do |t|
        t.breakpointsChanged.listen { |_| updateBreakpoints() }
      end

      @widget.clicked.listen do |f|
        puts "breakpoint clicked #{f}"
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
  end

end