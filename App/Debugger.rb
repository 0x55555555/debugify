require_relative 'RubyNotifier'

module App

  class Debugger
    def initialize(mw, log)
      @debugger = LldbDriver::Debugger.create()

      @log = log

      @isReady = false
      @ready = RubyNotifier.new
      @running = RubyNotifier.new
      @exited = RubyNotifier.new
      @processBegin = RubyNotifier.new
      @processEnd = RubyNotifier.new
      @targetChanged = RubyNotifier.new

      @mainwindow = mw
      @process = nil
    end

    attr_reader :ready, :running, :exited, :processBegin, :processEnd, :targetChanged
    attr_reader :target, :process

    def notReady()
      return running()
    end

    def load(t)
      @target = @debugger.loadTarget(t)
      @mainwindow.setTarget(@target)
      @targetChanged.call(@target)
    end

    def launch(args = [], env = [])
      err = LldbDriver::Error.new()
      raise "No target to launch" unless @target
      process = @target.launch(args, env, err)

      if (err.hasError())
        raise err.error()
      end

      setProcess(process)
      return process
    end

    def attach(pid)
      err = LldbDriver::Error.new()
      raise "No target to launch" unless @target
      process = @target.attach(pid, err)

      if (err.hasError())
        raise err.error()
      end

      setProcess(process)
      return process
    end

    def update()
      if (@process)
        @process.processEvents()
      end
      @ready.call(@mainwindow.process)
    end

  private
    def setProcess(p)
      @mainwindow.setProcess(p)
      @process = p

      if (@process)
        @processBegin.call(p)
        @process.stateChanged.listen{ |state| processStateChanged(state) }

        @process.outputAvailable.listen{ syncOutputs() }
        @process.errorAvailable.listen{ syncOutputs() }
      else
        @processEnd.call(p)
      end
    end

    def processStateChanged(state)
      if (state == Debugify::ProcessState[:Stopped])
        @isReady = true

        @process.threads.each do |t|
          reason = Debugify::Thread::StopReason[t.stopReason()]
          if (reason != :None)
            @log.log("Thread 0x#{t.id.to_s(16)} stopped with #{reason}")

            if (reason == :Breakpoint)
              brkpt, location = t.stopBreakpoint()

              @log.log "  at #{brkpt.id}.#{location.id} #{File.basename(location.file)}, #{location.line}"
            elsif (reason == :Exception)
              reason, str = t.stopException()
              @log.log "  #{str}"
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
        setProcess(nil)
      end
    end

    def syncOutputs()
      out, err = @process.getOutputs()

      @log.log(err) if err.length > 0
      @log.log(out) if out.length > 0
    end
  end

end