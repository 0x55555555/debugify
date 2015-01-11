#include "Module.h"
#include "ModuleImpl.h"
#include "CompileUnit.h"
#include "CompileUnitImpl.h"
#include "Utils.h"

namespace LldbDriver
{

Module::Module()
  {
  }

Module::~Module()
  {
  }

Eks::String Module::path() const
  {
  std::lock_guard<std::mutex> l(_impl->mutex);

  auto file = _impl->module.GetFileSpec();
  return fileSpecAsString(file);
  }

std::set<Eks::String> Module::files() const
  {
  std::set<Eks::String> set;

  auto addFile = [&set](auto spec)
    {
    auto s = fileSpecAsString(spec);
    if (s.length())
      {
      set.insert(s);
      }
    };

  std::lock_guard<std::mutex> l(_impl->mutex);
  for (uint32_t i = 0; i < _impl->module.GetNumCompileUnits(); ++i)
    {
    auto unit = _impl->module.GetCompileUnitAtIndex(i);
    addFile(unit.GetFileSpec());

    for (uint32_t j = 0; j < unit.GetNumSupportFiles(); ++j)
      {
      addFile(unit.GetSupportFileAtIndex(j));
      }
    }

  return set;
  }

const std::vector<std::shared_ptr<LldbDriver::CompileUnit>> &Module::compileUnits() const
  {
  if (!_impl->cachedUnits)
    {
    _impl->cachedUnits = true;
    std::lock_guard<std::mutex> l(_impl->mutex);

    auto cuCount = _impl->module.GetNumCompileUnits();
    for (size_t i = 0; i < cuCount; ++i)
      {
      auto cu = _impl->module.GetCompileUnitAtIndex(i);

      auto cuWrap = std::make_shared<CompileUnit>();
      cuWrap->_impl->myself = cuWrap;
      cuWrap->_impl->module = _impl->myself.lock();
      cuWrap->_impl->compileUnit = cu;

      _impl->compileUnits.push_back(cuWrap);
      }
    }

  return _impl->compileUnits;
  }


}
