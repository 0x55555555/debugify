#include "ModuleExplorer.h"
#include "Module.h"
#include "CompileUnit.h"
#include "Type.h"
#include "QtCore/QFileInfo"
#include "QtCore/QDateTime"
#include "QtWidgets/QHeaderView"
#include "QtWidgets/QTreeWidgetItem"
#include "QtWidgets/QLineEdit"
#include "QtWidgets/QVBoxLayout"
#include "QDebug"

class FilterHelper : public QSortFilterProxyModel
  {
public:
  FilterHelper(QObject *parent) : QSortFilterProxyModel(parent) { }

  bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const X_OVERRIDE
    {
    if (filterRegExp().isEmpty())
      {
      return true;
      }

    auto result = QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);

    return result || hasAcceptedChildren(sourceRow, sourceParent);
    }

  bool hasAcceptedChildren(int sourceRow, const QModelIndex &sourceParent) const
    {
    QModelIndex item = sourceModel()->index(sourceRow, 0, sourceParent);
    if (!item.isValid())
      {
      return false;
      }

    //check if there are children
    int childCount = item.model()->rowCount(item);
    if (childCount == 0)
      {
      return false;
      }

    for (int i = 0; i < childCount; ++i)
      {
      if (QSortFilterProxyModel::filterAcceptsRow(i, item))
        {
        return true;
        }

      if (hasAcceptedChildren(i, item))
        {
        return true;
        }
      }

    return false;
    }
  };

void ModuleWorker::loadFiles(const QStandardItem *item, const Module::Pointer &ptr)
  {
  auto vec = ptr->files();

  QStringList result;
  xForeach(auto &i, vec)
    {
    result << i.data();
    }

  qSort(result);

  emit loadedFiles(item, ptr, result);
  }

ModuleExplorer::ModuleExplorer(TypeManager *types)
    : _types(types)
  {
  qRegisterMetaType<Module::Pointer>();

  _widget = new QWidget();

  auto layout = new QVBoxLayout(_widget);
  layout->setContentsMargins(4, 4, 4, 4);
  layout->setSpacing(4);

  auto edit = new QLineEdit(_widget);
  edit->setPlaceholderText("Search");
  layout->addWidget(edit);
  connect(edit, SIGNAL(textChanged(QString)), this, SLOT(filterChanged(QString)));

  _tree = new QTreeView(_widget);
  layout->addWidget(_tree);
  _tree->setEditTriggers(QAbstractItemView::NoEditTriggers);
  _tree->header()->hide();

  _filter = new FilterHelper(this);
  _filter->setFilterKeyColumn(0);
  _tree->setModel(_filter);
  _filter->setSourceModel(this);

  _worker = new ModuleWorker;
  _workerThread = new QThread(this);
  _worker->moveToThread(_workerThread);
  _workerThread->start();

  connect(this, SIGNAL(loadFiles(const QStandardItem*,Module::Pointer)), _worker, SLOT(loadFiles(const QStandardItem*,Module::Pointer)));
  connect(_worker, SIGNAL(loadedFiles(const QStandardItem*,Module::Pointer,QStringList)), this, SLOT(loadedFiles(const QStandardItem*,Module::Pointer,QStringList)));

  connect(_tree, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(itemDoubleClicked(QModelIndex)));

  connect(types, SIGNAL(typeAdded(Module::Pointer, CachedType::Pointer, const Type*)), this, SLOT(typeAdded(Module::Pointer, CachedType::Pointer, const Type*)));
  }

ModuleExplorer::~ModuleExplorer()
  {
  _workerThread->quit();
  while (!_workerThread->isFinished())
    {
    QThread::currentThread()->msleep(1);
    }
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

QString ModuleExplorer::makeFileTooltip(const QString file, const QFileInfo &info)
  {
  auto lastModified = info.lastModified();
  QDateTime now = QDateTime::currentDateTime();

  QString type = "secs";
  qint64 to = lastModified.secsTo(now);
  if (to > 60)
    {
    to /= 60;
    type = "mins";
    }
  if (to > 60)
    {
    to /= 60;
    type = "hours";
    }
  if (to > 24)
    {
    to /= 24;
    type = "days";
    }
  if (to > 365)
    {
    to /= 365;
    type = "years";
    }

  return QString("Path: %1\nLast modified: (%2 %3 ago) %4")
      .arg(file)
      .arg(to)
      .arg(type)
      .arg(lastModified.toString());
  }

void ModuleExplorer::loadedFiles(const QStandardItem *constItem, const Module::Pointer &module, const QStringList &files)
  {
  auto item = const_cast<QStandardItem *>(constItem);

  if (!files.size())
    {
    return;
    }

  auto moduleData = _moduleMap[module];

  moduleData.filesItem = new QStandardItem("Files");
  item->appendRow(moduleData.filesItem);

  xForeach(auto file, files)
    {
    QFileInfo info(file);
    auto fileItem = new QStandardItem(info.fileName());
    fileItem->setData(file, PathRole);
    fileItem->setData(Source, TypeRole);
    fileItem->setData(QVariant::fromValue(module), ModuleRole);

    fileItem->setToolTip(makeFileTooltip(file, info));

    moduleData.filesItem->appendRow(fileItem);

    _fileMap[file] = fileItem;
    }

  moduleData.filesItem->sortChildren(0);
  }

void ModuleExplorer::itemDoubleClicked(const QModelIndex &index)
  {
  auto type = index.data(TypeRole);
  if (type == Source)
    {
    emit sourceFileActivated(index.data(ModuleRole).value<Module::Pointer>(), index.data(PathRole).toString());
    }
  else if (type == DataType)
    {
    emit dataTypeActivated(index.data(ModuleRole).value<Module::Pointer>(), index.data(PathRole).toString());
    }
  }

void ModuleExplorer::typeAdded(const Module::Pointer &module, const CachedType::Pointer &type, const Type *)
  {
  auto &item = _moduleMap[module];
  if (!item.moduleItem)
    {
    xAssertFail();
    return;
    }

  if (!item.typeItem)
    {	
    item.typeItem = new QStandardItem("Types");
    item.moduleItem->appendRow(item.typeItem);
    }

  auto parent = item.typeItem;
  auto it = item.items.find(type->parent);
  if (it != item.items.end())
    {
    parent = it->second;
    }

  auto typeItem = new QStandardItem(type->basename + type->specialisation);
  parent->appendRow(typeItem);
  typeItem->setData(type->path, PathRole);
  typeItem->setData(DataType, TypeRole);
  typeItem->setData(QVariant::fromValue(module), ModuleRole);
  item.items[type] = typeItem;

  typeItem->setToolTip(type->path);

  parent->sortChildren(0);
  }

void ModuleExplorer::filterChanged(const QString &filter)
  {
  Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive;
  QString strPattern = "^" + filter;
  QRegExp regExp(strPattern, caseSensitivity);

  _filter->setFilterRegExp(regExp);
  }

void ModuleExplorer::rebuildTree()
  {
  QStandardItemModel::clear();

  QStandardItem *systemItems = new QStandardItem("System");

  for(size_t i = 0; i < _target->moduleCount(); ++i)
    {
    auto module = _target->module(i);

    bool isSystem = false;
    auto item = new QStandardItem;
    buildModule(item, module, isSystem);

    if (isSystem)
      {
      systemItems->appendRow(QList<QStandardItem*>() << item);
      }
    else
      {
      appendRow(item);
      }

    _moduleMap[module] = { item, nullptr, nullptr, { } };
    }

  if (systemItems->hasChildren())
    {
    appendRow(QList<QStandardItem*>() << systemItems);
    }
  }

void ModuleExplorer::buildModule(QStandardItem *item, const Module::Pointer &module, bool &isSystem)
  {
  QString path = module->path().data();
  QFileInfo info(path);

  item->setData(Module, TypeRole);
  item->setData(path, PathRole);

  QString name(info.fileName());
  item->setText(name);

  item->setToolTip(makeFileTooltip(path, info));

  isSystem = isSystemModule(path, module);

  emit loadFiles(item, module);
  }
