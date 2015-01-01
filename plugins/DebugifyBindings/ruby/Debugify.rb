require_relative 'autogen/LldbDriverLibrary'

module LldbDriver

def self.addEnumerator(cls, nameSym, countSym, atSym)

  count = cls.instance_method(countSym)
  meth = cls.instance_method(atSym)

  cls.send(:define_method, nameSym) do
    result = []
    count.bind(self).call().times do |i|
      result << meth.bind(self).call(i)
    end
    return result
  end
end

addEnumerator(Target, :modules, :moduleCount, :moduleAt)

end

module Debugify

include LldbDriver


end