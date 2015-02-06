require_relative 'RubyNotifier'

module App

  class Debugger
    def initialize(mw, log, project)
      @debugger = LldbDriver::Debugger.create()

      @log = log

      @isReady = false
      @ready = RubyNotifier.new
      @running = RubyNotifier.new
      @exited = RubyNotifier.new
      @processBegin = RubyNotifier.new
      @processEnd = RubyNotifier.new
      @targetChanged = RubyNotifier.new
      @targetModulesChanged = RubyNotifier.new

      @mainwindow = mw
      @process = nil

      @project = project
      @project.install_handler(:modules, self)
      @extraModules = Set.new()
    end

    attr_reader :ready, :running, :exited, :processBegin, :processEnd, :targetChanged
    attr_reader :target, :process

    def notReady()
      return running()
    end

    def unload()
      if (!tryKill())
        return
      end
      @project.close()
      @extraModules = Set.new()
    end

    def load(t)
      unload()

      @target = @debugger.loadTarget(t)
      @targetChanged.call(@target)
      @project.reset(@target.path())
      # Set mainwindows target after the project is loaded so all modules are in place
      @mainwindow.setTarget(@target)
    end

    def addModule(mod)
      if (@extraModules.include?(mod))
        @log.log("Module already loaded")
      end

      if (!tryKill())
        return
      end

      @target.addModule(mod)
      @extraModules << mod
      @targetChanged.call(@target)
    end

    def launch(args = [], env = [])
      err = LldbDriver::Error.new()
      raise "No target to launch" unless @target
      return setupProcess(@target.launch(args, env, err), err)
    end

    def attach(pid)
      err = LldbDriver::Error.new()
      raise "No target to launch" unless @target
      return setupProcess(@target.attach(pid, err), err)
    end

    def connect(url)
      err = LldbDriver::Error.new()
      raise "No target to launch" unless @target
      return setupProcess(@target.connect(url, err), err)
    end

    def update()
      if (@process)
        @process.processEvents()
      end
      @ready.call(@mainwindow.process)
    end

    def tryKill()
      if (@process == nil)
        return true
      end

      res = UI::MessageBox.question(
        "Process Running",
        "Do you want to kill the running process?",
        UI::MessageBox::StandardButton[:Yes] | UI::MessageBox::StandardButton[:No]
      )

      if (res == UI::MessageBox::StandardButton[:No])
        return false
      end

      kill()
      return true
    end

    def kill()
      if (@process)
        @process.kill()
      end
    end

    # Methods for module serialisation
    def owns_value(key)
      return key == :modules
    end

    def deserialise(handler)
      if (handler.has_value(:modules))
        mods = handler.value(:modules)
        mods.each do |m|
          addModule(m)
        end
      end
    end

    def serialise(handler)
      if (handler.has_location(:exe))
        handler.set_value(:modules, @extraModules.to_a, :exe)
      end
    end

  private
    def setupProcess(process, err)
      if (err.hasError())
        raise err.error()
      end

      setProcess(process)
      return process
    end

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