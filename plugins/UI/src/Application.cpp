#include "Application.h"
#include "QStyleFactory"
#include "QFont"
#include "QDebug"

namespace UI
{

int g_argv = 0;
int &dummy(const QString &location)
  {
  QCoreApplication::addLibraryPath(location + "/");
  return g_argv;
  }

Application::Application(const QString &location)
    : QApplication(dummy(location), nullptr)
  {
  static Eks::Core core;

  setOrganizationName("JSoft");
  setApplicationName("Debugify");

  auto style = QStyleFactory::create("fusion");
  setStyle(style);

  auto f = font();
  f.setPointSize(12);
  setFont(f);
  }

Application::~Application()
  {
  }

}
