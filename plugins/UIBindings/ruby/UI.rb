require_relative 'autogen/UILibrary'

module UI

NotifierExtensions.install([
  InputNotifier,
  TargetNotifier,
  ProcessStateNotifier,
  ProcessNotifier,
  OutputNotifier,
  ClickNotifier,
  EditorNotifier,
  MarginClickNotifier
])

class Console < Qt::QWidget
  def puts(s)
    append("#{s}\n")
  end
end

end