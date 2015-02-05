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
#include "TypeManager.h"
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
class Dockable;

/// \expose unmanaged
X_DECLARE_NOTIFIER(OutputNotifier, std::function<void (QString)>);

/// \expose unmanaged
X_DECLARE_NOTIFIER(EditorNotifier, std::function<void (Editor *)>);

/// \expose unmanaged
X_DECLARE_NOTIFIER(AboutToCloseNotifier, std::function<void ()>);

/// \expose unmanaged
X_DECLARE_NOTIFIER(TypeNotifier, std::function<void (QString)>);

/// \expose unmanaged
X_DECLARE_NOTIFIER(DebugNotifier, std::function<void (QString)>);

/// \expose managed
class MainWindow : public QMainWindow
  {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  QString geometry() const;
  bool setGeometry(const QString &g);

  Terminal *addTerminal(const QString &n, bool toolbar);
  Console *addConsole(const QString &n, bool toolbar);
  EditableTextWindow *addEditor(const QString &n, bool toolbar);
  Dockable *addModuleExplorer(const QString &n, bool toolbar);

  ToolBar *addToolBar(const QString &n);

  QString getOpenFilename(const QString &caption, const QString &filter = QString());

  Menu *addMenu(const QString &name);

  void showDock(Dockable *w);
  void hideDock(Dockable *w);

  void setTarget(const Target::Pointer &tar);
  Target::Pointer target() const;

  void setProcess(const Process::Pointer &);
  Process::Pointer process() const;

  AboutToCloseNotifier *aboutToClose() { return &_aboutToClose; }

  EditorNotifier *editorOpened() { return &_editorOpened; }
  EditorNotifier *editorClosed() { return &_editorClosed; }

  TypeNotifier *typeAdded() { return &_typeAdded; }

  DebugNotifier *debugOutput() { return &_debugOutput; }

public slots:
  Editor *openFile(const QString &str, int line = -1);
  Editor *openType(const QString &);

private slots:
  void typeAdded(const Module::Pointer &, const UI::CachedType::Pointer &);
  void onError(const QString &str);
  void setStatusText(const QString &str);

  void closeFile(int tab);
  void openFile(const Module::Pointer &ptr, const QString &);
  void openType(const Module::Pointer &ptr, const QString &);

  void timerTick();

private:
  void checkError(const Error &err);

  void addEditor(Editor *editor);
  void focusEditor(Editor *editor);

  void closeEvent(QCloseEvent *event) X_OVERRIDE;

  static void log(QtMsgType, const QMessageLogContext &, const QString &);
  QtMessageHandler _oldHandler;

  Ui::MainWindow *ui;

  QHash<QString, Editor *> _editors;
  TypeManager *_types;

  Target::Pointer _target;

  Process::Pointer _process;

  EditorNotifier _editorOpened;
  EditorNotifier _editorClosed;

  TypeNotifier _typeAdded;

  DebugNotifier _debugOutput;

  AboutToCloseNotifier _aboutToClose;

  QTimer _timer;
  };

}
