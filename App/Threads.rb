require_relative 'TextWindow'

module App

  class Threads

    attr_reader :widget
    
    def initialize(mainWindow, debugger)
      @debugger = debugger
      @widget = mainWindow.addEditor("Threads")

      @widget.clicked.listen do |p|
        threadList = mainWindow.process.threads
        selected = threadList.find do |t| 
          next t.id == p.to_i
        end
        mainWindow.process.selectThread(selected)
        debugger.update()
      end

      debugger.ready.listen do |process|
        threadList = mainWindow.process.threads
        threadStrings = threadList.map { |t| formatThread(t) }
        selected = threadList.find_index { |t| t.isCurrent }
        @widget.setContents(alternatingColourList(threadStrings, { :selected => selected }))
      end

      debugger.running.listen do |process|
        @widget.setContents("")
      end
    end

    def formatThread(t)
      frameTop = t.frames[0]
      lineData = nil
      if (frameTop.hasLineNumber)
        lineData = "line #{frameTop.lineNumber}"
      end

      return link(t.id, "0x#{t.id.to_s(16)} #{t.name()} #{frameTop.functionName} #{lineData}")
    end

    include TextWindow
  end

end