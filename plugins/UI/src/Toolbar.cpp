#include "ToolBar.h"
#include "QAction"
#include "QDebug"

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

  connect(a, &QAction::triggered, [clicked]()
    {
    try
      {
      clicked();
      }
    catch (const std::exception &e)
      {
      qWarning() << e.what();
      }
    });

  return a;
  }
}
