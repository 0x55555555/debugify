#include "Application.h"
#include "QStyleFactory"
#include "QFont"

namespace UI
{

int g_argv = 0;

Application::Application()
    : QApplication(g_argv, nullptr)
  {
  static Eks::Core core;

  setOrganizationName("JSoft");
  setApplicationName("Debugger");

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
