#pragma once
#include "QToolBar"

namespace UI
{

/// \expose
class ToolBar : public QToolBar
  {
  Q_OBJECT

public:
  ToolBar();
  ~ToolBar();

  QAction *addAction(const QString &str, const std::function<void()> &clicked);
  };

}
