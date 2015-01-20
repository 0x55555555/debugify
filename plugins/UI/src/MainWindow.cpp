#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "ModuleExplorer.h"
#include "Error.h"
#include "QDebug"
#include "QSettings"
#include "QFileDialog"
#include "QDockWidget"
#include "FileEditor.h"
#include "TypeEditor.h"
#include "Terminal.h"
#include "ToolBar.h"
#include "EditableTextWindow.h"
#include <array>

namespace UI
{

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  _targetToolbar(nullptr),
  _processToolbar(nullptr),
  _processState(Process::State::Invalid)
  {
  ui->setupUi(this);
  connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeFile(int)));
    
  qRegisterMetaType<Module::Pointer>();

  _debugger = Debugger::create();

  _types = new TypeManager();

  auto dock = new QDockWidget();
  dock->setObjectName("ModuleExplorer");
  dock->setWindowTitle("Modules");
  _moduleExplorer = new ModuleExplorer(_types);
  dock->setWidget(_moduleExplorer->widget());
  addDockWidget(Qt::LeftDockWidgetArea, dock);
  connect(_moduleExplorer, SIGNAL(sourceFileActivated(Module::Pointer,QString)), this, SLOT(openFile(Module::Pointer,QString)));
  connect(_moduleExplorer, SIGNAL(dataTypeActivated(Module::Pointer,QString)), this, SLOT(openType(Module::Pointer,QString)));

  connect(ui->actionLoad_Target, &QAction::triggered, [this]()
    {
    QString fileName = QFileDialog::getOpenFileName(this, "Load Executable Target");
    if (fileName.isEmpty())
      {
      return;
      }

    QSettings settings;
    QStringList files = settings.value("recentTargets").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentFiles)
      {
      files.removeLast();
      }
    settings.setValue("recentTargets", files);

    setTarget(_debugger->loadTarget(fileName.toUtf8().data()));
    });

  QMenu *recentMenu = new QMenu;
  ui->actionRecent_Targets->setMenu(recentMenu);

  connect(recentMenu, &QMenu::aboutToShow, [this, recentMenu]()
    {
    QSettings settings;
    QStringList files = settings.value("recentTargets").toStringList();

    recentMenu->setEnabled(!files.isEmpty());
    recentMenu->clear();

    QFontMetrics metric(font());
    xForeach(auto recent, files)
      {
      auto act = recentMenu->addAction(metric.elidedText(recent, Qt::ElideLeft, 250));
      connect(act, &QAction::triggered, [this, recent]
        {
        setTarget(_debugger->loadTarget(recent.toUtf8().data()));
        });
      }
    });


  connect(&_timer, SIGNAL(timeout()), this, SLOT(timerTick()));
  _timer.start(100);
  }

MainWindow::~MainWindow()
  {
  delete ui;
  }

Terminal *MainWindow::addTerminal(const QString &name)
  {
  auto terminal = new Terminal();
  auto dock = new QDockWidget();
  dock->setObjectName(name);
  dock->setWindowTitle(name);
  dock->setWidget(terminal);
  addDockWidget(Qt::BottomDockWidgetArea, dock);

  return terminal;
  }

Console *MainWindow::addConsole(const QString &name)
  {
  auto console = new Console();
  auto dock = new QDockWidget();
  dock->setObjectName(name);
  dock->setWindowTitle(name);
  dock->setWidget(console);
  addDockWidget(Qt::BottomDockWidgetArea, dock);

  return console;
  }

EditableTextWindow *MainWindow::addEditor(const QString &name)
  {
  auto editor = new EditableTextWindow();
  auto dock = new QDockWidget();
  dock->setObjectName(name);
  dock->setWindowTitle(name);
  dock->setWidget(editor);
  addDockWidget(Qt::LeftDockWidgetArea, dock);

  return editor;
  }

ToolBar *MainWindow::addToolBar(const QString &n)
  {
  auto toolbar = new ToolBar;
  toolbar->setObjectName(n);

  QMainWindow::addToolBar(Qt::TopToolBarArea, toolbar);
  return toolbar;
  }

void MainWindow::showDock(QWidget *w)
  {
  if (auto dock = qobject_cast<QDockWidget *>(w->parent()))
    {
    dock->show();
    return;
    }

  qWarning() << "Invalid dock widget passed to show " << w;
  }

void MainWindow::hideDock(QWidget *w)
  {
  if (auto dock = qobject_cast<QDockWidget *>(w->parent()))
    {
    dock->hide();
    return;
    }

  qWarning() << "Invalid dock widget passed to hide " << w;
  }

Target::Pointer MainWindow::target() const
  {
  return _target;
  }

Process::Pointer MainWindow::process() const
  {
  return _process;
  }

void MainWindow::setTarget(const Target::Pointer &tar)
  {
  _moduleExplorer->setTarget(tar);
  _types->setTarget(tar);

  _target = tar;
  _targetNotifier(_target);
  }

void MainWindow::setProcess(const Process::Pointer &ptr)
  {
  _process = ptr;
  _processNotifier(_process);

  syncState(_process ? _process->currentState() : Process::State::Invalid);
  }

void MainWindow::onError(const QString &str)
  {
  qWarning() << str;
  }

void MainWindow::setStatusText(const QString &str)
  {
  ui->statusbar->showMessage(str);
  }

void MainWindow::processStateChanged(Process::State state)
  {
  _processStateNotifier(state);

  if (state == Process::State::Invalid)
    {
    setProcess(nullptr);
    }
  }

void MainWindow::closeFile(int tab)
  {
  auto editor = ui->tabWidget->widget(tab);
  auto key = _editors.key(static_cast<Editor*>(editor), QString());
  if (!key.isEmpty())
    {
    _editors.remove(key);
    }

  ui->tabWidget->removeTab(tab);
  }

Editor *MainWindow::openFile(const QString &file, int line)
  {
  if (auto editor = _editors.value(FileEditor::makeKey(file), nullptr))
    {
    focusEditor(editor);
    return editor;
    }

  auto editor = new FileEditor(file);
  addEditor(editor);

  if (line != -1)
    {
    editor->focusOnLine(line);
    }

  return editor;
  }

void MainWindow::openFile(const Module::Pointer &, const QString &file)
  {
  openFile(file);
  }

void MainWindow::openType(const Module::Pointer &, const QString &type)
  {
  openType(type);
  }

void MainWindow::openType(const QString &type)
  {
  if (auto editor = _editors.value(TypeEditor::makeKey(type), nullptr))
    {
    focusEditor(editor);
    return;
    }

  try
    {
    auto editor = new TypeEditor(_types, type);

    connect(editor, SIGNAL(selectType(QString)), this, SLOT(openType(QString)));
    connect(editor, SIGNAL(selectFile(QString,int)), this, SLOT(openFile(QString,int)));
    addEditor(editor);
    }
  catch (const NoSuchTypeException &)
    {
    // ignore and return
    }
  }

void MainWindow::onProcessOutput(const QString &str)
  {
  _stdout(str);
  }

void MainWindow::onProcessError(const QString &str)
  {
  _stderr(str);
  }

void MainWindow::timerTick()
  {
  if (!_process)
    {
    syncState(Process::State::Invalid);
    return;
    }

  _process->processEvents();

  auto newState = _process->currentState();
  bool postStateChange =
      newState == Process::State::Stopped ||
      newState == Process::State::Crashed ||
      newState == Process::State::Detached ||
      newState == Process::State::Exited ||
      newState == Process::State::Suspended;
  if (!postStateChange)
    {
    syncState(newState);
    }

  auto forwardOutput = [this](auto type, const auto &send)
  {
    std::array<char, 256> output;
    while (size_t read = _process->getOutput(type, output.data(), output.size()-1))
      {
      xAssert(read <= (output.size()-1));
      output[read] = '\0';
      send(output.data());
      }
    };

  forwardOutput(Process::OutputType::Output, [this](auto str) { onProcessOutput(str); });
  forwardOutput(Process::OutputType::Error, [this](auto str) { onProcessError(str); });

  if (postStateChange)
    {
    syncState(newState);
    }
  }

void MainWindow::syncState(Process::State type)
  {
  if (_processState == type)
    {
    return;
    }

  if (_process)
    {
    emit setStatusText(
      QString("Process %1 [%2 %3]")
        .arg(_process->target()->path().data())
        .arg(_process ? _process->processID() : 0)
        .arg(Process::getStateString(type).data())
      );
    }

  _processState = type;

  processStateChanged(type);
  }

void MainWindow::checkError(const Error &err)
  {
  if (err.hasError())
    {
    onError(err.error().data());
    }
  }

void MainWindow::addEditor(Editor *editor)
  {
  int tabIndex = ui->tabWidget->addTab(editor, editor->title());
  ui->tabWidget->setTabToolTip(tabIndex, editor->path());
  _editors[editor->key()] = editor;
  focusEditor(editor);

  _editorOpened(editor);
  }

void MainWindow::focusEditor(Editor *editor)
  {
  ui->tabWidget->setCurrentWidget(editor);
  }

}
