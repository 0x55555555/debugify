require_relative 'RubyNotifier'

module App

  class Debugger
    def initialize(mw, log)
      # Move this here at some point
      #@debugger = LldbDriver::Debugger.create()

      @log = log

      @isReady = false
      @ready = RubyNotifier.new
      @running = RubyNotifier.new
      @exited = RubyNotifier.new

      @mainwindow = mw
      @process = nil

      mw.processChanged.listen do |p|
        @process = p

        if (@process)
          @process.stateChanged.listen do |state|
            processStateChanged(state)
          end
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
      if (@process)
        @process.processEvents()
      end
      @ready.call(@mainwindow.process)
    end

  private
    def processStateChanged(state)
      if (state == Debugify::ProcessState[:Stopped])
        @isReady = true

        puts "Process Stopped"
        @process.threads.each do |t|
          reason = Debugify::Thread::StopReason[t.stopReason()]
          if (reason != :None)
            @log.log("Thread 0x#{t.id.to_s(16)} stopped with #{reason}")

            if (reason == :Breakpoint)
              brkpt, location = t.stopBreakpoint()

              @log.log "  at #{brkpt.id}.#{location.id} #{File.basename(location.file)}, #{location.line}"
            end
          end
        end

        @ready.call(@process)
      end

      if (state == Debugify::ProcessState[:Running])
        @isReady = false
        @running.call(@process)
      end

      if (state == Debugify::ProcessState[:Exited])
        @isReady = false
        # need to ensure callbacks are aware ready is false
        @running.call(@process)
        @exited.call(@process)
        @process = nil
      end
    end
  end

end