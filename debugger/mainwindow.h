#pragma once
#include "UiGlobal.h"
#include "QMainWindow"
#include "QToolBar"
#include "QTimer"
#include "TargetToolbar.h"
#include "ProcessToolbar.h"
#include "Debugger.h"
#include "Target.h"
#include "Process.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
  {
  Q_OBJECT

public:
  enum
    {
    MaxRecentFiles = 10
    };

  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void onError(const QString &str);
  void setStatusText(const QString &str);

  void onProcessStarted(const Process::Pointer &);
  void onProcessEnded(const Process::Pointer &);

private:
  void checkError(const Error &err);

  void setTarget(const Target::Pointer &tar);
  void setProcess(const Process::Pointer &tar);

  Ui::MainWindow *ui;

  Debugger::Pointer _debugger;

  TargetToolbar *_targetToolbar;
  ProcessToolbar *_processToolbar;
  };

