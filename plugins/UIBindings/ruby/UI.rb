require_relative 'autogen/UILibrary'

module UI

NotifierExtensions.install([
  InputNotifier,
  OutputNotifier,
  ClickNotifier,
  EditorNotifier,
  MarginClickNotifier,
  AboutToCloseNotifier,
  AboutToShowNotifier,
])

class Console < Qt::QWidget
  def puts(s)
    append("#{s}\n")
  end
end

end