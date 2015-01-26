require_relative 'TextWindow'

module App

  class Values
    include TextWindow

    attr_reader :widget
    
    def initialize(mainWindow, debugger)
      @widget = mainWindow.addEditor("Values")

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
      args = frame.arguments().map { |v| formatValue(:argument, v) }
      locals = frame.locals().map { |v| formatValue(:local, v) }

      @widget.setContents(alternatingColourList(args.concat(locals)))
    end

    def formatValue(type, v)
      tStr = type == :argument ? "A" : "L"
      "#{tStr} #{v.name} #{v.type.name}(#{v.value})"
    end
  end
end