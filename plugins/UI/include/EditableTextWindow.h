#pragma once
#include "QTextEdit"
#include "XGlobal.h"
#include "Notifier.h"

namespace UI
{

/// \expose unmanaged
DECLARE_NOTIFIER(ClickNotifier, std::function<void (QString)>);

/// \expose
class EditableTextWindow : public QTextEdit
  {
  Q_OBJECT
public:
  EditableTextWindow();
  ~EditableTextWindow();

  void setContents(const QString &);

  ClickNotifier *clicked() { return &_clicked; }

private:
  void mouseDoubleClickEvent(QMouseEvent *e) X_OVERRIDE;

  ClickNotifier _clicked;
  };

}
