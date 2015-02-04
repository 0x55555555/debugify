require_relative 'TextWindow'

module App

  class CallStack
    include TextWindow

    attr_reader :widget
    
    def initialize(mainWindow, debugger)
      @widget = mainWindow.addEditor("Call Stack", false)

      @widget.clicked.listen do |f, x, y|
        thread = mainWindow.process.selectedThread

        newFrame = thread.frames[f.to_i]
        thread.selectFrame(newFrame)
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

  end

end