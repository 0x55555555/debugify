require_relative '../ruby/Debugify.rb'

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

dbg = Debugify::Debugger.create()
target = dbg.loadTarget("/Volumes/User/llvm/build-debugger-Desktop-Debug/qtc_Desktop-debug/test.qtc_Desktop/test")

puts target.path
