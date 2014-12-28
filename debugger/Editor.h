#pragma once
#include "QWidget"

class Editor : public QWidget
  {
public:

  virtual QString path() const = 0;
  virtual QString key() const = 0;
  virtual QString title() const = 0;
  };
