require_relative 'autogen/LldbDriverLibrary'

module LldbDriver

class EnumerationWrapper
  include Enumerable

  def initialize(countFn, atFn)
    @count = countFn
    @at = atFn
  end

  def length
    return @count.call()
  end

  def [](i)
    @at.call(i)
  end

  def each
    length.times do |i|
      yield(self[i])
    end
  end
end

def self.addEnumerator(cls, nameSym, countSym, atSym)

  count = cls.instance_method(countSym)
  meth = cls.instance_method(atSym)

  cls.send(:define_method, nameSym) do
    return EnumerationWrapper.new(count.bind(self), meth.bind(self))
  end
end

addEnumerator(Target, :modules, :moduleCount, :moduleAt)
addEnumerator(Process, :threads, :threadCount, :threadAt)
addEnumerator(Thread, :frames, :frameCount, :frameAt)

end

module Debugify

include LldbDriver


end