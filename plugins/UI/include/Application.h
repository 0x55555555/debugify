#pragma once
#include "QtWidgets/QApplication"
#include "XCore.h"

namespace UI
{

/// \expose managed
class Application : public QApplication
  {
public:
  Application();
  ~Application();
  };

}
