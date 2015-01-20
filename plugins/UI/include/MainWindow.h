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
#include "Utilities/XNotifier.h"

namespace Ui {
class MainWindow;
}

namespace UI
{

class Terminal;
class Console;
class EditableTextWindow;
class TypeManager;
class Editor;
class TargetToolbar;
class ProcessToolbar;
class ModuleExplorer;
class ToolBar;

/// \expose unmanaged
X_DECLARE_NOTIFIER(TargetNotifier, std::function<void (Target::Pointer)>);

/// \expose unmanaged
X_DECLARE_NOTIFIER(ProcessNotifier, std::function<void (Process::Pointer)>);

/// \expose unmanaged
X_DECLARE_NOTIFIER(ProcessStateNotifier, std::function<void (Process::State)>);

/// \expose unmanaged
X_DECLARE_NOTIFIER(OutputNotifier, std::function<void (QString)>);

/// \expose unmanaged
X_DECLARE_NOTIFIER(EditorNotifier, std::function<void (Editor *)>);

/// \expose managed
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

  Terminal *addTerminal(const QString &n);
  Console *addConsole(const QString &n);
  EditableTextWindow *addEditor(const QString &n);

  ToolBar *addToolBar(const QString &n);

  void showDock(QWidget *w);
  void hideDock(QWidget *w);

  TargetNotifier *targetChanged() { return &_targetNotifier; }
  Target::Pointer target() const;

  void setProcess(const Process::Pointer &);
  ProcessNotifier *processChanged() { return &_processNotifier; }
  ProcessStateNotifier *processStateChanged() { return &_processStateNotifier; }
  Process::Pointer process() const;

  OutputNotifier *output() { return &_stdout; }
  OutputNotifier *errors() { return &_stderr; }

  EditorNotifier *editorOpened() { return &_editorOpened; }

public slots:
  Editor *openFile(const QString &str, int line = -1);

private slots:
  void onError(const QString &str);
  void setStatusText(const QString &str);
  void processStateChanged(Process::State);

  void closeFile(int tab);
  void openFile(const Module::Pointer &ptr, const QString &);
  void openType(const Module::Pointer &ptr, const QString &);
  void openType(const QString &);

  void onProcessOutput(const QString &);
  void onProcessError(const QString &);

  void timerTick();

private:
  void checkError(const Error &err);

  void setTarget(const Target::Pointer &tar);

  void addEditor(Editor *editor);
  void focusEditor(Editor *editor);

  void syncState(Process::State type);

  Ui::MainWindow *ui;

  Debugger::Pointer _debugger;

  QHash<QString, Editor *> _editors;

  TargetToolbar *_targetToolbar;
  ProcessToolbar *_processToolbar;
  TypeManager *_types;
  ModuleExplorer *_moduleExplorer;

  TargetNotifier _targetNotifier;
  Target::Pointer _target;

  ProcessStateNotifier _processStateNotifier;
  ProcessNotifier _processNotifier;
  Process::Pointer _process;
  Process::State _processState;

  OutputNotifier _stdout;
  OutputNotifier _stderr;

  EditorNotifier _editorOpened;

  QTimer _timer;
  };

}
