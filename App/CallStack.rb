require_relative 'TextWindow'

module App

  class CallStack

    attr_reader :widget
    
    def initialize(mainWindow, debugger)
      @widget = mainWindow.addEditor("Call Stack")

      @widget.setContents("<a href=\"plink\">Plink</a><br><a href=\"plonk\">Plonk</a>")

      @widget.clicked.listen do |f|
        thread = mainWindow.process.selectedThread

        thread.selectFrame(thread.frames[f.to_i])
        debugger.update()
      end

      debugger.ready.listen do |process|
        thread = mainWindow.process.selectedThread
        frames = thread.frames

        frameStrings = frames.map { |t| formatFrame(t) }
        selected = frames.find_index { |t| t.isCurrent }
        @widget.setContents(alternatingColourList(frameStrings, { :selected => selected }))
      end

      debugger.running.listen do |process|
        @widget.setContents("")
      end
    end

    def formatFrame(frame)
      lineData = nil
      if (frame.hasLineNumber)
        lineData = "line #{frame.lineNumber}"
      end

      return link(frame.id, "#{frame.functionName} #{lineData}")
    end

    include TextWindow
  end

end