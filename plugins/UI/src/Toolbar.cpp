#include "ToolBar.h"
#include "QAction"

namespace UI
{

ToolBar::ToolBar()
  {

  }

ToolBar::~ToolBar()
  {

  }

QAction *ToolBar::addAction(const QString &str, const std::function<void()> &clicked)
  {
  QAction *a = QToolBar::addAction(str);

  connect(a, &QAction::triggered, clicked);

  return a;
  }
}
