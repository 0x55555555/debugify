#include "TypeManager.h"
#include "CompileUnit.h"
#include "QtCore/QThread"
#include "Containers/XStringBuilder.h"
#include "QDebug"

namespace UI
{

CachedType::CachedType()
    : definitions(Eks::Core::defaultAllocator())
  {
  }

TypeManager::TypeManager()
  {
  qRegisterMetaType<LldbDriver::Type*>();
  qRegisterMetaType<CachedType::Pointer>();

  _worker = new TypeManagerWorker(this);
  _workerThread = new QThread();
  _worker->moveToThread(_workerThread);
  _workerThread->start();

  connect(this, SIGNAL(loadTypes(Module::Pointer)), _worker, SLOT(loadTypes(Module::Pointer)));

  connect(_worker, SIGNAL(loadedType(Module::Pointer, UI::CachedType::Pointer, const Type*)), this, SLOT(loadedType(Module::Pointer, UI::CachedType::Pointer, const Type*)));
  }

void TypeManager::setTarget(const Target::Pointer &tar)
  {
  _target = tar;
  _types.clear();

  if (!_target)
    {
    return;
    }

  for(size_t i = 0; i < _target->moduleCount(); ++i)
    {
    auto module = _target->moduleAt(i);
    emit loadTypes(module);
    }
  }

void TypeManager::registerType(const std::shared_ptr<CachedType> &cached)
  {
  std::lock_guard<std::mutex> lock(_typeLock);
  xAssert(!_types.contains(cached->path));
  _types[cached->path] = cached;
  }

std::shared_ptr<CachedType> TypeManager::findType(const QString &str)
  {
  std::lock_guard<std::mutex> lock(_typeLock);	

  return _types.value(str);
  }

void TypeManager::loadedType(const Module::Pointer &module, const UI::CachedType::Pointer &cached, const Type *type)
  {
  emit typeAdded(module, cached, type);
  }

TypeManagerWorker::TypeManagerWorker(TypeManager *mgr)
    : _manager(mgr)
  {
  }

template <typename T> T skipBracesUntil(const T &beg, const T &end, Eks::String::Char left, Eks::String::Char right, Eks::String::Char until, T &lastBrace)
  {
  size_t level = 0;
  for (auto it = beg; it != end; ++it)
    {
    if (*it == left)
      {
      ++level;
      }
    if (*it == right)
      {
      --level;
      lastBrace = it;
      }
    if (level == 0 && *it == until)
      {
      return it;
      }
    }

  return end;
  }

void splitPath(const Eks::String &path, Eks::StringBuilder &parent, Eks::StringBuilder &basename, Eks::StringBuilder &specialisation)
  {
  // note the braces are the wrong way round - reverse iteration.
  Eks::String::const_reverse_iterator lastBrace = path.rend();
  auto it = skipBracesUntil(path.rbegin(), path.rend(), '>', '<', ':', lastBrace);

  auto nameBegin = it.base();
  auto basenameEnd = lastBrace.base();

  if (basenameEnd == path.begin())
    {
    basenameEnd = path.end();
    }
  else
    {
    basenameEnd -= 1;
    }

  if (it != path.rend())
    {
    auto parentEnd = nameBegin - 2;

    parent.adjust([&](Eks::String &str)
      {
      str.insert(str.begin(), path.begin(), parentEnd);
      });
    }

  basename.adjust([&](Eks::String &str)
    {
    str.insert(str.begin(), nameBegin, basenameEnd);
    });

  specialisation.adjust([&](Eks::String &str)
    {
    str.insert(str.begin(), basenameEnd, path.end());
    });
  }

void TypeManagerWorker::loadTypes(const Module::Pointer &ptr)
  {
  auto units = ptr->compileUnits();

  xForeach(const auto &unit, units)
    {
    xForeach(const auto &type, unit->types())
      {
      if (type.isDataStructure())
        {
        loadType(ptr, type);
        }
      }
    }
  }

void TypeManagerWorker::loadType(const Module::Pointer &ptr, const Type &t)
  {
  auto source = t.name();
  QString qSource = source.data();
  if (auto type = _manager->findType(qSource))
    {
    type->definitions << t;
    return;
    }

  createType(ptr, qSource, source, &t);
  }

std::shared_ptr<CachedType> TypeManagerWorker::findOrCreateType(const Module::Pointer &module, const QString &qstr, const Eks::String &str)
  {
  if (qstr.isEmpty())
    {
    return nullptr;
    }

  if (auto type = _manager->findType(qstr))
    {
    return type;
    }

  return createType(module, qstr, str, nullptr);
  }

std::shared_ptr<CachedType> TypeManagerWorker::createType(const Module::Pointer &module, const QString &qStr, const Eks::String &str, const Type *t)
  {
  std::shared_ptr<CachedType> type = std::make_shared<CachedType>();

  Eks::StringBuilder path, basename, specialisation;
  splitPath(str, path, basename, specialisation);

  auto parent = findOrCreateType(module, path.data(), path);
  type->parent = parent;
  type->path = qStr;
  type->basename = basename.data();
  type->specialisation = specialisation.data();

  if (t)
    {
    type->definitions << *t;
    }

  _manager->registerType(type);
  emit loadedType(module, type, t);

  return type;
  }

}
