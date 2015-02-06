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

static MainWindow *g_mw = 0;

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
  {
  xAssert(g_mw == 0);
  g_mw = this;
  ui->setupUi(this);
  connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeFile(int)));
    
  qRegisterMetaType<Module::Pointer>();

  _types = new TypeManager();
  connect(_types, SIGNAL(typeAdded(Module::Pointer,UI::CachedType::Pointer)), this, SLOT(typeAdded(Module::Pointer,UI::CachedType::Pointer)));

  connect(&_timer, SIGNAL(timeout()), this, SLOT(timerTick()));
  _timer.start(100);

  _oldHandler = qInstallMessageHandler(log);
  }

MainWindow::~MainWindow()
  {
  delete ui;
  auto x = qInstallMessageHandler(_oldHandler);
  (void)x;
  xAssert(x == log);

  xAssert(g_mw == this);
  g_mw = nullptr;
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

Terminal *MainWindow::addTerminal(const QString &name, bool toolbar)
  {
  auto dock = new Terminal(toolbar);
  dock->setObjectName(name);
  dock->setWindowTitle(name);
  addDockWidget(Qt::BottomDockWidgetArea, dock);

  return dock;
  }

Console *MainWindow::addConsole(const QString &name, bool toolbar)
  {
  auto dock = new Console(toolbar);
  dock->setObjectName(name);
  dock->setWindowTitle(name);
  addDockWidget(Qt::BottomDockWidgetArea, dock);

  return dock;
  }

EditableTextWindow *MainWindow::addEditor(const QString &name, bool toolbar)
  {
  auto dock = new EditableTextWindow(toolbar);
  dock->setObjectName(name);
  dock->setWindowTitle(name);
  addDockWidget(Qt::LeftDockWidgetArea, dock);

  return dock;
  }

Dockable *MainWindow::addModuleExplorer(const QString &n, bool toolbar)
  {
  auto dock = new ModuleExplorerDock(_types, toolbar);
  dock->setObjectName(n);
  dock->setWindowTitle(n);

  addDockWidget(Qt::LeftDockWidgetArea, dock);

  connect(dock->explorer(), SIGNAL(sourceFileActivated(Module::Pointer,QString)), this, SLOT(openFile(Module::Pointer,QString)));
  connect(dock->explorer(), SIGNAL(dataTypeActivated(Module::Pointer,QString)), this, SLOT(openType(Module::Pointer,QString)));

  return dock;
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
  auto menu = new Menu(this);
  menu->setTitle(name);

  ui->menubar->addMenu(menu);
  return menu;
  }

void MainWindow::showDock(Dockable *dock)
  {
  if (dock)
    {
    dock->show();
    return;
    }

  qWarning() << "Invalid dock widget passed to show " << dock;
  }

void MainWindow::hideDock(Dockable *dock)
  {
  if (dock)
    {
    dock->hide();
    return;
    }

  qWarning() << "Invalid dock widget passed to hide " << dock;
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
  _types->setTarget(tar);

  ui->tabWidget->clear();
  _editors.clear();

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

void MainWindow::log(QtMsgType t, const QMessageLogContext &c, const QString &m)
  {
  if (g_mw)
    {
    g_mw->_oldHandler(t, c, m);

    (*g_mw->debugOutput())(m + "\n");
    }
  }
}
