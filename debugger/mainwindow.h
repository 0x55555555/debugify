#pragma once
#include "UiGlobal.h"
#include "QMainWindow"
#include "QToolBar"
#include "QTimer"
#include "QPointer"
#include "Debugger.h"
#include "Target.h"
#include "Process.h"
#include "Module.h"

namespace Ui {
class MainWindow;
}

class TypeManager;
class Editor;
class TargetToolbar;
class ProcessToolbar;
class ModuleExplorer;

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
  void processStateChanged(Process::State );

  void onProcessStarted(const Process::Pointer &);
  void onProcessEnded(const Process::Pointer &);

  void closeFile(int tab);
  void openFile(const Module::Pointer &ptr, const QString &);
  void openType(const Module::Pointer &ptr, const QString &);
  void openType(const QString &);

private:
  void checkError(const Error &err);

  void setTarget(const Target::Pointer &tar);
  void setProcess(const Process::Pointer &tar);

  void addEditor(Editor *editor);
  void focusEditor(Editor *editor);

  Ui::MainWindow *ui;

  Debugger::Pointer _debugger;

  QHash<QString, Editor *> _editors;

  TargetToolbar *_targetToolbar;
  ProcessToolbar *_processToolbar;
  TypeManager *_types;
  ModuleExplorer *_moduleExplorer;
  };

