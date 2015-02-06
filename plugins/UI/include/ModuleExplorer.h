#pragma once
#include "UiGlobal.h"
#include "Target.h"
#include "Module.h"
#include "Dockable.h"
#include "TypeManager.h"
#include "QtWidgets/QTreeView"
#include "QtCore/QSortFilterProxyModel"
#include "QtGui/QStandardItemModel"
#include "QtCore/QThread"
#include <set>

class QFileInfo;

namespace UI
{

class ModuleWorker;

class ModuleExplorer : public QStandardItemModel
  {
  Q_OBJECT
public:
  ModuleExplorer(TypeManager *types);
  ~ModuleExplorer();

  QWidget *widget() { return _widget; }

  void setTarget(const Target::Pointer &ptr);

  enum
    {
    TypeRole = Qt::UserRole,
    PathRole,
    ModuleRole
    };

  enum ItemType
    {
    Module,
    Source,
    DataType
    };

signals:
  void sourceFileActivated(const Module::Pointer &, QString file);
  void dataTypeActivated(const Module::Pointer &, QString file);

  void loadFiles(const Module::Pointer &);

private slots:
  void loadedFiles(const Module::Pointer &, const QStringList &str);
  void itemDoubleClicked(const QModelIndex &);
  void typeAdded(const Module::Pointer &module, const UI::CachedType::Pointer &);
  void typeDeclarationAdded(const Module::Pointer &module, const UI::CachedType::Pointer &, const Type *t);
  void filterChanged(const QString &filter);

private:
  void clearTree();
  void rebuildTree();
  void buildModule(QStandardItem *module, const Module::Pointer &, bool &isSystem);
  bool isSystemModule(const QString &path, const Module::Pointer &);
  QString makeFileTooltip(const QString file, const QFileInfo &info);
  void insertTypeReference(const Module::Pointer &module, QStandardItem *parent, const CachedType::Pointer &type);

  struct ModuleData
    {
    QStandardItem *moduleItem = nullptr;
    QStandardItem *filesItem = nullptr;
    QStandardItem *typeItem = nullptr;
    std::unordered_map<CachedType::Pointer, QStandardItem *> items;
    };

  QHash<QString, QStandardItem*> _fileMap;
  QMultiMap<QString, CachedType::Pointer> _unownedTypeMap;
  std::unordered_map<Module::Pointer, ModuleData> _moduleMap;

  QWidget *_widget;
  QTreeView *_tree;
  QSortFilterProxyModel *_filter;
  Target::Pointer _target;

  QThread *_workerThread;
  ModuleWorker *_worker;
  TypeManager *_types;
  };

/// \expose
class ModuleExplorerDock : public Dockable
  {
  Q_OBJECT
public:
  ModuleExplorerDock(TypeManager *types, bool toolbar);

  void setTarget(const Target::Pointer &ptr);

  ModuleExplorer *explorer() { return _explorer; }

private:
  ModuleExplorer *_explorer;
  };

class ModuleWorker : public QObject
  {
  Q_OBJECT

public slots:
  void loadFiles(const Module::Pointer &);

signals:
  void loadedFiles(const Module::Pointer &, const QStringList &str);
  void loadedType(const Module::Pointer &, const Type &str);
  };

}

Q_DECLARE_METATYPE(Module::Pointer)
