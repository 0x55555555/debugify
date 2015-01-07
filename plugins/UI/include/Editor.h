#pragma once
#include "QWidget"

namespace UI
{

/// \expose unmanaged
class Editor : public QWidget
  {
  Q_OBJECT

public:
  virtual QString path() const = 0;
  virtual QString key() const = 0;
  virtual QString title() const = 0;
  };

}
