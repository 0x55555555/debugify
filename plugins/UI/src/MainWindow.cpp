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
#include "Menu.h"
#include "EditableTextWindow.h"
#include <array>

namespace UI
{

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
  {
  ui->setupUi(this);
  connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeFile(int)));
    
  qRegisterMetaType<Module::Pointer>();

  _types = new TypeManager();

  auto dock = new QDockWidget();
  dock->setObjectName("ModuleExplorer");
  dock->setWindowTitle("Modules");
  _moduleExplorer = new ModuleExplorer(_types);
  dock->setWidget(_moduleExplorer->widget());
  addDockWidget(Qt::LeftDockWidgetArea, dock);
  connect(_moduleExplorer, SIGNAL(sourceFileActivated(Module::Pointer,QString)), this, SLOT(openFile(Module::Pointer,QString)));
  connect(_moduleExplorer, SIGNAL(dataTypeActivated(Module::Pointer,QString)), this, SLOT(openType(Module::Pointer,QString)));
  connect(_types, SIGNAL(typeAdded(Module::Pointer,UI::CachedType::Pointer)), this, SLOT(typeAdded(Module::Pointer,UI::CachedType::Pointer)));

  connect(&_timer, SIGNAL(timeout()), this, SLOT(timerTick()));
  _timer.start(100);
  }

MainWindow::~MainWindow()
  {
  delete ui;
  }

QString MainWindow::geometry() const
  {
  QByteArray out;
    {
    QDataStream str(&out, QIODevice::ReadWrite);
    str << saveGeometry() << saveState();
    }

  return out.toBase64();
  }

bool MainWindow::setGeometry(const QString &g)
  {
  QByteArray geo, state;
    {
    auto data = QByteArray::fromBase64(g.toUtf8());
    QDataStream str(&data, QIODevice::ReadWrite);

    str >> geo >> state;
    }

  bool res1 = restoreGeometry(geo);
  bool res2 = restoreState(state);

  return res1 && res2;
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

QString MainWindow::getOpenFilename(const QString &caption, const QString &filter)
  {
  return QFileDialog::getOpenFileName(this, caption, QString(), filter);
  }

Menu *MainWindow::addMenu(const QString &name)
  {
  auto menu = new Menu;
  menu->setTitle(name);

  ui->menubar->addMenu(menu);
  return menu;
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
  }

void MainWindow::setProcess(const Process::Pointer &ptr)
  {
  _process = ptr;
  }

void MainWindow::typeAdded(const Module::Pointer &, const UI::CachedType::Pointer &type)
  {
  _typeAdded(type->path);
  }

void MainWindow::onError(const QString &str)
  {
  qWarning() << str;
  }

void MainWindow::setStatusText(const QString &str)
  {
  ui->statusbar->showMessage(str);
  }

void MainWindow::closeFile(int tab)
  {
  auto editor = static_cast<Editor *>(ui->tabWidget->widget(tab));
  _editorClosed(editor);

  auto key = _editors.key(editor, QString());
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

Editor *MainWindow::openType(const QString &type)
  {
  if (auto editor = _editors.value(TypeEditor::makeKey(type), nullptr))
    {
    focusEditor(editor);
    return editor;
    }

  try
    {
    auto editor = new TypeEditor(_types, type);

    connect(editor, SIGNAL(selectType(QString)), this, SLOT(openType(QString)));
    connect(editor, SIGNAL(selectFile(QString,int)), this, SLOT(openFile(QString,int)));
    addEditor(editor);
    return editor;
    }
  catch (const NoSuchTypeException &)
    {
    // ignore and return
    }

  return nullptr;
  }


void MainWindow::timerTick()
  {
  if (!_process)
    {
    return;
    }

  _process->processEvents();
        // process may end during the above call
  if (!_process)
    {
    return;
    }
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

void MainWindow::closeEvent(QCloseEvent *event)
  {
  _aboutToClose();

  event->accept();
  }
}
