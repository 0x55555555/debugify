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
  TypeNotifier,
  ChangedNotifier,
])

class Console < UI::Dockable
  def puts(s)
    append("#{s}\n")
  end
end

UIC_PATH = File.dirname(__FILE__) + "/../../../App/ui/"

end