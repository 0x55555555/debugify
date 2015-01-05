#pragma once
#include "UiGlobal.h"
#include "Module.h"
#include "Target.h"
#include "Type.h"
#include "QtCore/QObject"
#include "QtCore/QHash"

namespace UI
{

class TypeManagerWorker;

class CachedType
  {
public:
  CachedType();

  typedef std::shared_ptr<CachedType> Pointer;

  QString path;
  Pointer parent;
  QString basename;
  QString specialisation;
  Eks::Vector<Type, 16> definitions;
  };

class TypeManager : public QObject
  {
  Q_OBJECT

public:
  TypeManager();

  void setTarget(const Target::Pointer &tar);

  void registerType(const std::shared_ptr<CachedType> &);
  std::shared_ptr<CachedType> findType(const QString &str);

public slots:
  void loadedType(const Module::Pointer &module, const UI::CachedType::Pointer &cached, const Type *type);

signals:
  void loadTypes(const Module::Pointer &ptr);
  void typeAdded(const Module::Pointer &module, const UI::CachedType::Pointer &, const Type *t);

private:
  QThread *_workerThread;
  TypeManagerWorker *_worker;
  std::mutex _typeLock;
  QHash<QString, std::shared_ptr<CachedType>> _types;
  Target::Pointer _target;
  };

class TypeManagerWorker : public QObject
  {
  Q_OBJECT
public:
  TypeManagerWorker(TypeManager *mgr);

public slots:
  void loadTypes(const Module::Pointer &ptr);
  void loadType(const Module::Pointer &ptr, const Type &t);

signals:
  void loadedType(const Module::Pointer &, const UI::CachedType::Pointer &type, const Type *);

private:
  std::shared_ptr<CachedType> findOrCreateType(const Module::Pointer &module, const QString &qstr, const Eks::String &str);
  std::shared_ptr<CachedType> createType(const Module::Pointer &module, const QString &qstr, const Eks::String &str, const Type *t);
  TypeManager *_manager;
  };

}

Q_DECLARE_METATYPE(Type*);
Q_DECLARE_METATYPE(UI::CachedType::Pointer);

