#include "Menu.h"
#include "QAction"

namespace UI
{

Menu::Menu()
  {
  connect(this, &QMenu::aboutToShow, this, &Menu::onAboutToShow);
  }

Menu::~Menu()
  {
  }

void Menu::clear()
  {
  QMenu::clear();
  }

Menu *Menu::addMenu(const QString &str)
  {
  Menu *a = new Menu();
  a->setTitle(str);
  QMenu::addMenu(a);

  return a;
  }

QAction *Menu::addAction(const QString &str, const std::function<void()> &clicked)
  {
  QAction *a = QMenu::addAction(str);

  connect(a, &QAction::triggered, clicked);

  return a;
  }

void Menu::onAboutToShow()
  {
  _aboutToShow();
  }
}
