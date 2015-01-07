require_relative 'RubyNotifier'

module App

  class Debugger
    def initialize(mw)
      @ready = RubyNotifier.new
      @running = RubyNotifier.new

      @mainwindow = mw

      mw.processStateChanged.listen do |p|
        process = mw.process
        if (p == Debugify::Process::State[:Stopped])
          @ready.call(process)
        end

        if (p == Debugify::Process::State[:Running])
          @running.call(process)
        end
      end
    end

    attr_reader :ready, :running

    def update()
      @ready.call(@mainwindow.process)
    end
  end

end