#!/usr/bin/ruby

require_relative '../sample/autogen_Debugify/DebugifyLibrary.rb'

#puts Debugify.foo(5, 4)
#puts Debugify.bar(4, 6)

val = Debugify.foo(5, 4)

puts val
puts val.boink
puts val.boink
puts val.boink

#val.foo(val)