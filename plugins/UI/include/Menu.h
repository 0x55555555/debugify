#pragma once
#include "QMenu"
#include "Utilities/XNotifier.h"

namespace UI
{

/// \expose unmanaged
X_DECLARE_NOTIFIER(AboutToShowNotifier, std::function<void ()>);

/// \expose
class Menu : public QMenu
  {
  Q_OBJECT

public:
  Menu(QWidget *owner);
  ~Menu();

  void clear();
  Menu *addMenu(const QString &str);
  QAction *addAction(const QString &str, const std::function<void()> &clicked);

  void exec();

  AboutToShowNotifier *aboutToShow() { return &_aboutToShow; }

private slots:
  void onAboutToShow();

private:
  AboutToShowNotifier _aboutToShow;
  };

}
