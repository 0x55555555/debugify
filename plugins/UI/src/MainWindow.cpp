#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "TargetToolbar.h"
#include "ProcessToolbar.h"
#include "ModuleExplorer.h"
#include "Error.h"
#include "QDebug"
#include "QSettings"
#include "QFileDialog"
#include "QDockWidget"
#include "FileEditor.h"
#include "TypeEditor.h"
#include "Terminal.h"
#include "EditableTextWindow.h"

namespace UI
{

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  _targetToolbar(nullptr),
  _processToolbar(nullptr)
  {
  ui->setupUi(this);
  connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeFile(int)));
    
  qRegisterMetaType<Module::Pointer>();

  _debugger = Debugger::create();

  _targetToolbar = new TargetToolbar();
  _targetToolbar->setVisible(false);
  connect(_targetToolbar, SIGNAL(error(QString)), this, SLOT(onError(QString)));
  connect(_targetToolbar, SIGNAL(processStarted(Process::Pointer)), this, SLOT(onProcessStarted(Process::Pointer)));
  addToolBar(_targetToolbar);

  _processToolbar = new ProcessToolbar();
  _processToolbar->setVisible(false);
  connect(_processToolbar, SIGNAL(error(QString)), this, SLOT(onError(QString)));
  connect(_processToolbar, SIGNAL(statusUpdate(QString)), this, SLOT(setStatusText(QString)));
  connect(_processToolbar, SIGNAL(stateChanged(Process::State)), this, SLOT(processStateChanged(Process::State)));
  connect(_processToolbar, SIGNAL(processOutput(QString)), this, SLOT(onPrcocessOutput(QString)));
  connect(_processToolbar, SIGNAL(processError(QString)), this, SLOT(onPrcocessError(QString)));
  addToolBar(_processToolbar);

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
  _targetToolbar->setVisible(false);

  _moduleExplorer->setTarget(tar);
  _types->setTarget(tar);

  _target = tar;
  _targetNotifier(_target);

  if (tar)
    {
    _targetToolbar->setTarget(tar);
    _targetToolbar->setVisible(true);
    }
  }

void MainWindow::setProcess(const Process::Pointer &ptr)
  {
  _processToolbar->setVisible(false);

  _process = ptr;
  _processNotifier(_process);

  if (ptr)
    {
    _processToolbar->setProcess(ptr);
    _processToolbar->setVisible(true);
    }
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
    _processToolbar->setVisible(false);
    setProcess(nullptr);
    }
  }

void MainWindow::onProcessStarted(const Process::Pointer &process)
  {
  setProcess(process);
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

void MainWindow::openFile(const Module::Pointer &, const QString &file)
  {
  if (auto editor = _editors.value(FileEditor::makeKey(file), nullptr))
    {
    focusEditor(editor);
    return;
    }

  auto editor = new FileEditor(file);
  addEditor(editor);
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
    addEditor(editor);
    }
  catch (const NoSuchTypeException &)
    {
    // ignore and return
    }
  }

void MainWindow::onPrcocessOutput(const QString &str)
  {
  _stdout(str);
  }

void MainWindow::onPrcocessError(const QString &str)
  {
  _stderr(str);
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
  }

void MainWindow::focusEditor(Editor *editor)
  {
  ui->tabWidget->setCurrentWidget(editor);
  }

}
