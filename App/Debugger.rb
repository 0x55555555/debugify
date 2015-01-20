require_relative 'RubyNotifier'

module App

  class Debugger
    def initialize(mw)
      # Move this here at some point
      #@debugger = LldbDriver::Debugger.create()

      @ready = RubyNotifier.new
      @running = RubyNotifier.new
      @exited = RubyNotifier.new

      @mainwindow = mw

      mw.processStateChanged.listen do |p|
        process = mw.process
        if (p == Debugify::Process::State[:Stopped])
          @ready.call(process)
        end

        if (p == Debugify::Process::State[:Running])
          @running.call(process)
        end

        if (p == Debugify::Process::State[:Exited])
          @exited.call(process)
        end
      end
    end

    attr_reader :ready, :running, :exited

    def launch(args = [], env = [])
      err = LldbDriver::Error.new()
      target = @mainwindow.target
      raise "No target to launch" unless target
      process = @mainwindow.target.launch(args, env, err)

      if (err.hasError())
        raise err.error()
      end

      @mainwindow.setProcess(process)
      return process
    end

    def end()
      @mainwindow.setProcess(nil)
    end

    def update()
      @ready.call(@mainwindow.process)
    end
  end

end