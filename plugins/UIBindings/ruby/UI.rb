require_relative 'autogen/UILibrary'

module UI

class Terminal < Qt::QWidget

  #def input(&tester)
  #  puts tester
  #  onInput(tester)
  #end
end

module NotifierExtensions

  def listen(&x)
    super(x)
  end
end

class InputNotifier
  prepend NotifierExtensions
end

class TargetNotifier
  prepend NotifierExtensions
end

class ProcessNotifier
  prepend NotifierExtensions
end

end