
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
      return "<a href=\"#{t.id}\">0x#{t.id.to_s(16)} #{t.name()}<a>"
    end

    SELECTED = "#e8e762"
    ALTERNATE_A = "#ffffff"
    ALTERNATE_B = "#f7f7f7"

    def alternatingColourList(list, options)
      rows = list.map.with_index do |t, i|
        col = nil
        if (i == options[:selected])
          col = SELECTED
        else
          col = i % 2 == 0 ? ALTERNATE_A : ALTERNATE_B
        end

        colouredRow(t, col)
      end

      return rows.join()
    end

    def colouredRow(str, col)
      return "<div style=\"background-color: #{col}; width: 100%\">#{str}</div>"
    end
  end

end