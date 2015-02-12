require_relative 'TextWindow'

module App

  class Values
    include TextWindow

    attr_reader :widget
    
    def initialize(mainWindow, debugger)
      @widget = mainWindow.addEditor("Values", false)

      debugger.ready.listen do |process|
        thread = mainWindow.process.selectedThread
        if (thread)
          frame = thread.selectedFrame
          if (frame)
            updateValues(frame)
          end
        end
      end

      debugger.notReady.listen do |process|
        @widget.setContents("")
      end
    end

    def updateValues(frame)
      values = []

      frame.arguments().map { |v| formatValue(:argument, v, values) }
      frame.locals().map { |v| formatValue(:local, v, values) }

      @widget.setContents(alternatingColourList(values))
    end

    Types = { :argument => "A", :local => "L", :child => " "}

    def formatValue(type, v, values, depth = 0)
      tStr = Types[type]
      children = v.mightHaveChildren
      start = children ? "+" : " "
      
      values << "#{'&nbsp;' * 2 * depth}#{start} #{tStr} #{v.name} #{v.type.name}(#{v.value})"

      if (children)
        v.children.each do |c|
          formatValue(:child, c, values, depth + 1)
        end
      end
    end
  end
end