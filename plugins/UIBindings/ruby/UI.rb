require_relative 'autogen/UILibrary'

module UI

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

class ProcessStateNotifier
  prepend NotifierExtensions
end

class ProcessNotifier
  prepend NotifierExtensions
end

class OutputNotifier
  prepend NotifierExtensions
end

class ClickNotifier
  prepend NotifierExtensions
end

class EditorNotifier
  prepend NotifierExtensions
end

class MarginClickNotifier
  prepend NotifierExtensions
end

class Console < Qt::QWidget
  def puts(s)
    append("#{s}\n")
  end
end

end