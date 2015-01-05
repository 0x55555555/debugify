require 'pry'
require 'strscan'

module App

  class DebuggerTerminal
    class IOWrapper
      def initialize(term, old)
        @term = term
        @old = old
      end

      attr_accessor :pry

      def print(*more)
        str = ""
        more.each do |e|
          str += e.to_s
        end
        write(str)
      end

      def flush()
        @old.flush()
      end

      def tty?
        return true
      end

      def write(str)
        @old.write(str)
        @term.append(str)
      end

      def prompt()
        raise "No pry instance" unless @pry
        write(@pry.select_prompt)
      end
    end

    def initialize(mainWindow)
      @terminal = mainWindow.addTerminal("Debugify Debug Console")

      $stdout = IOWrapper.new(@terminal, $stdout)
      $stderr = IOWrapper.new(@terminal, $stderr)

      @pry = initPry($stdout)
      $stdout.pry = @pry

      @terminal.input.listen do |inp|
        @pry.eval inp
        $stdout.prompt
      end

      ENV['TERM']='ansi'
      puts "Debugger is up."
      $stdout.prompt
    end

  private
    def initPry(output)
      Pry.config.pager = false
      return Pry.new(
        :output => output,
        :target => TOPLEVEL_BINDING,
        :commands => Pry.config.commands
      )
    end
  end

end