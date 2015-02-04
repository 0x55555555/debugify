#pragma once
#include "Dockable.h"
#include "XGlobal.h"
#include "Utilities/XNotifier.h"
#include "QTextEdit"

namespace UI
{

class EditableTextWindow;

/// \expose unmanaged
X_DECLARE_NOTIFIER(ClickNotifier, std::function<void (QString, int, int)>);

class EditableTextEdit : public QTextEdit
  {
public:
  EditableTextEdit(EditableTextWindow *window);

  void mouseDoubleClickEvent(QMouseEvent *e) X_OVERRIDE;
  void contextMenuEvent(QContextMenuEvent *e) X_OVERRIDE;

private:
  EditableTextWindow *_window;
  };

/// \expose
class EditableTextWindow : public Dockable
  {
  Q_OBJECT

public:
  EditableTextWindow(bool toolbar);
  ~EditableTextWindow();

  void setContents(const QString &);

  ClickNotifier *clicked() { return &_clicked; }
  ClickNotifier *contextMenu() { return &_contextMenu; }

private:
  EditableTextEdit *_edit;

  ClickNotifier _clicked;
  ClickNotifier _contextMenu;
  };

}
