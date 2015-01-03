require 'pry'
require 'pry-rescue'

ENV['DYLD_LIBRARY_PATH'] = '/Volumes/User/llvm/llvm-build/Debug+Asserts//lib/:/Volumes/User/llvm/ruby-debugger/test/../..//build-debugger-Desktop_Qt_5_4_0_clang_64bit-Debug/qtc_Desktop_Qt_5_4_0_clang_64bit-debug//LldbDriver.qtc_Desktop:/Volumes/User/llvm/ruby-debugger/test/../..//build-debugger-Desktop_Qt_5_4_0_clang_64bit-Debug/qtc_Desktop_Qt_5_4_0_clang_64bit-debug//UI.qtc_Desktop'
require_relative '../plugins/DebugifyBindings/ruby/Debugify.rb'
require_relative '../plugins/UIBindings/ruby/UI.rb'


#Pry.rescue do
=begin
  puts Debugify.foo(5, 4)
  puts Debugify.bar(4, 6)
  puts Debugify::Test.new(2)

  val = Debugify.foo(5, 4)

  puts val
  puts val.boink
  puts val.boink
  puts val.boink

  val.foo(val)
=end

  class IOWrapper
    def initialize(term, old)
      @term = term
      @old = old
    end

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
      write("> ")
    end
  end

  app = UI::Application.new()
  mw = UI::MainWindow.new()

  mw.show()

  $stdout = IOWrapper.new(mw.terminal, $stdout)
  $stderr = IOWrapper.new(mw.terminal, $stderr)

  mw.terminal.input.listen do |inp|
    puts Pry.run_command(inp)
    $stdout.prompt
  end
  puts "Debugger is up."
  $stdout.prompt

  app.execute()

  dbg = Debugify::Debugger.create()
  target = dbg.loadTarget("/Volumes/User/llvm/build-debugger-Desktop_Qt_5_4_0_clang_64bit-Debug/qtc_Desktop_Qt_5_4_0_clang_64bit-debug/test.qtc_Desktop_Qt_5_4_0_clang_64bit/test")

  #puts target
  #puts target.path

  #puts target.modules.map { |m| m.path }
#end