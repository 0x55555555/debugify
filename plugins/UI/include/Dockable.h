#pragma once
#include "QDockWidget"

class QVBoxLayout;

namespace UI
{

class ToolBar;

/// \expose unmanaged
class Dockable : public QDockWidget
  {
  Q_OBJECT

public:
  Dockable(bool toolbar);

  ToolBar *toolBar();

  void setWidget(QWidget *widget);

private:
  ToolBar *_toolbar;
  QVBoxLayout *_layout;
  QWidget *_widget;
  QWidget *_dockedWidget;
  };

}
