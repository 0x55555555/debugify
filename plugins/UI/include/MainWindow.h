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
class Menu;

/// \expose unmanaged
X_DECLARE_NOTIFIER(OutputNotifier, std::function<void (QString)>);

/// \expose unmanaged
X_DECLARE_NOTIFIER(EditorNotifier, std::function<void (Editor *)>);

/// \expose unmanaged
X_DECLARE_NOTIFIER(AboutToCloseNotifier, std::function<void ()>);

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

  QString getOpenFilename(const QString &caption, const QString &filter = QString());

  Menu *addMenu(const QString &name);

  void showDock(QWidget *w);
  void hideDock(QWidget *w);

  void setTarget(const Target::Pointer &tar);
  Target::Pointer target() const;

  void setProcess(const Process::Pointer &);
  Process::Pointer process() const;

  AboutToCloseNotifier *aboutToClose() { return &_aboutToClose; }

  OutputNotifier *output() { return &_stdout; }
  OutputNotifier *errors() { return &_stderr; }

  EditorNotifier *editorOpened() { return &_editorOpened; }

public slots:
  Editor *openFile(const QString &str, int line = -1);

private slots:
  void onError(const QString &str);
  void setStatusText(const QString &str);

  void closeFile(int tab);
  void openFile(const Module::Pointer &ptr, const QString &);
  void openType(const Module::Pointer &ptr, const QString &);
  void openType(const QString &);

  void onProcessOutput(const QString &);
  void onProcessError(const QString &);

  void timerTick();

private:
  void checkError(const Error &err);


  void addEditor(Editor *editor);
  void focusEditor(Editor *editor);

  void closeEvent(QCloseEvent *event) X_OVERRIDE;

  Ui::MainWindow *ui;

  QHash<QString, Editor *> _editors;
;
  TypeManager *_types;
  ModuleExplorer *_moduleExplorer;

  Target::Pointer _target;

  Process::Pointer _process;

  OutputNotifier _stdout;
  OutputNotifier _stderr;

  EditorNotifier _editorOpened;

  AboutToCloseNotifier _aboutToClose;

  QTimer _timer;
  };

}
