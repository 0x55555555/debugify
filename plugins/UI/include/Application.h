#pragma once
#include "QtWidgets/QApplication"
#include "XCore.h"
#include "Containers/XStringSimple.h"

namespace UI
{

/// \expose managed
class Application : public QApplication
  {
public:
  Application(const QString &location);
  ~Application();
  };

}

