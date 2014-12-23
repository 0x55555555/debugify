#pragma once
#include "UiGlobal.h"
#include "Target.h"
#include "Module.h"
#include "QtWidgets/QTreeView"
#include "QtGui/QStandardItemModel"

class ModuleExplorer : public QTreeView, public QStandardItemModel
  {
public:
  ModuleExplorer();

  void setTarget(const Target::Pointer &ptr);

private:
  void rebuildTree();
  bool isSystemModule(const QString &path, const Module::Pointer &);

  Target::Pointer _target;
  };
