#include "ModuleExplorer.h"
#include "Module.h"
#include "QtCore/QFileInfo"
#include "QtCore/QDateTime"
#include "QtWidgets/QHeaderView"
#include "QtWidgets/QTreeWidgetItem"

ModuleExplorer::ModuleExplorer()
  {
  header()->hide();
  setModel(this);
  }

void ModuleExplorer::setTarget(const Target::Pointer &tar)
  {
  _target = tar;

  rebuildTree();
  }

bool ModuleExplorer::isSystemModule(const QString &path, const Module::Pointer &)
  {
#ifdef X_OSX
  return path.startsWith("/usr/lib") || path.startsWith("/System/Library");
#else
# error Define system library paths?
#endif
  }

void ModuleExplorer::rebuildTree()
  {
  QStandardItemModel::clear();

  QStandardItem *systemItems = new QStandardItem("System");

  for(size_t i = 0; i < _target->moduleCount(); ++i)
    {
    auto module = _target->module(i);

    QString path = module->path().data();
    QFileInfo info(path);

    QString name(info.fileName());
    auto item = new QStandardItem(name);

    QString tooltip = QString("Path: %1\nLast modified: %2")
      .arg(path)
      .arg(info.lastModified().toString());

    item->setToolTip(tooltip);

    if (isSystemModule(path, module))
      {
      systemItems->appendRow(QList<QStandardItem*>() << item);
      }
    else
      {
      appendRow(item);
      }
    }

  if (systemItems->hasChildren())
    {
    appendRow(QList<QStandardItem*>() << systemItems);
    }
  }
