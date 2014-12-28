#!/usr/bin/ruby

require '/Volumes/User/llvm/ruby-debugger/plugins/DebugifyBindings/test/DebugifyBindings'

puts Debugify.foo(5, 4)
puts Debugify.bar(4, 6)

val = Debugify.foo(5, 4)

val.foo(val)