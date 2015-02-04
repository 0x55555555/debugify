#include "Dockable.h"
#include "QVBoxLayout"
#include "ToolBar.h"

namespace UI
{

Dockable::Dockable(bool toolbar)
    : _dockedWidget(nullptr)
  {
  _widget = new QWidget();
  _layout = new QVBoxLayout(_widget);
  _layout->setContentsMargins(0, 0, 0, 0);
  _layout->setSpacing(0);
  QDockWidget::setWidget(_widget);

  if (toolbar)
    {
    _toolbar = new ToolBar;
    _layout->addWidget(_toolbar);
    }
  }

ToolBar *Dockable::toolBar()
  {
  return _toolbar;
  }

void Dockable::setWidget(QWidget *widget)
  {
  if (_dockedWidget)
    {
    _layout->removeWidget(_dockedWidget);
    }

  _dockedWidget = widget;

  if (_dockedWidget)
    {
    _layout->addWidget(_dockedWidget);
    }
  }

}
