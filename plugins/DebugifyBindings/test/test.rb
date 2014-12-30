require_relative '../sample/RubyBindings/Debugify.rb'

puts Debugify.foo(5, 4)
puts Debugify.bar(4, 6)
puts Debugify::Test.new(2)

val = Debugify.foo(5, 4)

puts val
puts val.boink
puts val.boink
puts val.boink

val.foo(val)