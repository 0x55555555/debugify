#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "TargetToolbar.h"
#include "ProcessToolbar.h"
#include "ModuleExplorer.h"
#include "Error.h"
#include "QDebug"
#include "QSettings"
#include "QFileDialog"
#include "QDockWidget"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  _targetToolbar(nullptr),
  _processToolbar(nullptr)
  {
  ui->setupUi(this);

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
  addToolBar(_processToolbar);

  auto dock = new QDockWidget();
  dock->setObjectName("ModuleExplorer");
  dock->setWindowTitle("Modules");
  _moduleExplorer = new ModuleExplorer;
  dock->setWidget(_moduleExplorer);
  addDockWidget(Qt::LeftDockWidgetArea, dock);

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

  /*auto mod = ->addAction("List Modules");
  connect(mod, &QAction::triggered,
    [this]()
    {
    auto debugFile = [](lldb::SBFileSpec filespec)
      {
      filespec.ResolveExecutableLocation();

      if (!filespec.GetDirectory() || !filespec.GetFilename())
        {
        return false;
        }

      std::cout << "thing: " << filespec.GetDirectory() << "/" << filespec.GetFilename() << std::endl;
      return true;
      };

    std::cout << "Symbols" << std::endl;
    auto num = _debugData->currentTarget.GetNumModules();
    for (uint32_t i = 0; i < num; ++i)
      {
      auto mod = _debugData->currentTarget.GetModuleAtIndex(i);

      for (auto j = 0U; j < mod.GetNumCompileUnits(); ++j)
        {
        auto unit = mod.GetCompileUnitAtIndex(j);
        if (debugFile(unit.GetFileSpec()))
          {
          std::cout << unit.GetNumSupportFiles() << " support files";
          for (auto k = 0U; k < unit.GetNumSupportFiles(); ++k)
            {
            debugFile(unit.GetSupportFileAtIndex(k));
            }
          }
        }
      }
    });*/
  }

MainWindow::~MainWindow()
  {
  delete ui;
  }

void MainWindow::setTarget(const Target::Pointer &tar)
  {
  _targetToolbar->setVisible(false);

  _moduleExplorer->setTarget(tar);

  if (tar)
    {
    _targetToolbar->setTarget(tar);
    _targetToolbar->setVisible(true);
    }
  }

void MainWindow::setProcess(const Process::Pointer &ptr)
  {
  _processToolbar->setVisible(false);

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
  if (state == Process::State::Invalid)
    {
    _processToolbar->setVisible(false);
    }
  }

void MainWindow::onProcessStarted(const Process::Pointer &process)
  {
  setProcess(process);
  }

void MainWindow::onProcessEnded(const Process::Pointer &)
  {
  setProcess(nullptr);
  }

void MainWindow::checkError(const Error &err)
  {
  if (err.hasError())
    {
    onError(err.error().data());
    }
  }

int main(int argc, char *argv[])
  {
  Eks::Core core;
  QApplication app(argc, argv);
  app.setOrganizationName("JSoft");
  app.setApplicationName("Debugger");

  MainWindow w;

  w.show();

  return app.exec();
  }
