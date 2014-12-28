#pragma once
#include "UiGlobal.h"
#include "Target.h"
#include "Module.h"
#include "TypeManager.h"
#include "QtWidgets/QTreeView"
#include "QtCore/QSortFilterProxyModel"
#include "QtGui/QStandardItemModel"
#include "QtCore/QThread"
#include <set>

class QFileInfo;
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

  void loadFiles(const QStandardItem* , const Module::Pointer &);

private slots:
  void loadedFiles(const QStandardItem *module, const Module::Pointer &, const QStringList &str);
  void itemDoubleClicked(const QModelIndex &);
  void typeAdded(const Module::Pointer &module, const CachedType::Pointer &, const Type *t);
  void filterChanged(const QString &filter);

private:
  void rebuildTree();
  void buildModule(QStandardItem *module, const Module::Pointer &, bool &isSystem);
  bool isSystemModule(const QString &path, const Module::Pointer &);
  QString makeFileTooltip(const QString file, const QFileInfo &info);

  struct ModuleData
    {
    QStandardItem *moduleItem = nullptr;
    QStandardItem *filesItem = nullptr;
    QStandardItem *typeItem = nullptr;
    std::unordered_map<CachedType::Pointer, QStandardItem *> items;
    };

  QHash<QString, QStandardItem*> _fileMap;
  std::unordered_map<Module::Pointer, ModuleData> _moduleMap;

  QWidget *_widget;
  QTreeView *_tree;
  QSortFilterProxyModel *_filter;
  Target::Pointer _target;

  QThread *_workerThread;
  ModuleWorker *_worker;
  TypeManager *_types;
  };

Q_DECLARE_METATYPE(Module::Pointer)

class ModuleWorker : public QObject
  {
  Q_OBJECT

public slots:
  void loadFiles(const QStandardItem *item, const Module::Pointer &);

signals:
  void loadedFiles(const QStandardItem *item, const Module::Pointer &, const QStringList &str);
  void loadedType(const Module::Pointer &, const Type &str);
  };
