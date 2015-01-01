#include "CompileUnit.h"
#include "CompileUnitImpl.h"
#include "Type.h"
#include "TypeImpl.h"

namespace LldbDriver
{

CompileUnit::CompileUnit()
  {
  }

CompileUnit::~CompileUnit()
  {
  }

const std::shared_ptr<Module> &CompileUnit::module() const
  {
  return _impl->module;
  }

const std::vector<Type> &CompileUnit::types() const
  {
  if (!_impl->cachedTypes)
    {
    _impl->cachedTypes = true;
    std::lock_guard<std::mutex> l(_impl->mutex);

    auto types = _impl->compileUnit.GetTypes();
    for (size_t i = 0; i < types.GetSize(); ++i)
      {
      _impl->types.push_back(Type::Impl::make(_impl->myself.lock(), types.GetTypeAtIndex(i)));
      }
    }

  return _impl->types;
  }

}
