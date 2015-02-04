#include "Menu.h"
#include "QAction"
#include "QDebug"

namespace UI
{

Menu::Menu(QWidget *owner)
    : QMenu(owner)
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
  Menu *a = new Menu(this);
  a->setTitle(str);
  QMenu::addMenu(a);

  return a;
  }

QAction *Menu::addAction(const QString &str, const std::function<void()> &clicked)
  {
  QAction *a = QMenu::addAction(str);

  connect(a, &QAction::triggered, [clicked]()
    {
    try
      {
      clicked();
      }
    catch(const std::exception &e)
      {
      qWarning() << e.what();
      }
    });

  return a;
  }

void Menu::exec()
  {
  QMenu::exec(QCursor::pos());
  }

void Menu::onAboutToShow()
  {
  _aboutToShow();
  }
}
