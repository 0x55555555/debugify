#pragma once
#include "UiGlobal.h"
#include "Target.h"
#include "Process.h"
#include <QtWidgets/QToolBar>

class TargetToolbar : public QToolBar
  {
  Q_OBJECT

public:
  TargetToolbar(const Target::Pointer &target);

signals:
  void processStarted(const Process::Pointer &ptr);
  void error(const QString &);

private:
  void checkError(const Error &err);

  Target::Pointer _target;
  };
