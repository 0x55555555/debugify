#pragma once
#include "UiGlobal.h"
#include "Process.h"
#include <QtWidgets/QToolBar>
#include <QtCore/QTimer>

class ProcessToolbar : public QToolBar
  {
  Q_OBJECT

public:
  ProcessToolbar();

  void setProcess(const Process::Pointer &process);

signals:
  void error(const QString &);
  void statusUpdate(const QString &);
  void stateChanged(Process::State newState);

private slots:
  void togglePause();

private:
  void syncState(LldbDriver::Process::State type);
  void checkError(const Error &err);

  Process::Pointer _process;
  Process::State _processState;

  QAction *_stop;
  QTimer _timer;
  };

