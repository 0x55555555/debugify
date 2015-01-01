#include "CompileUnit.h"
#include "Module.h"
#include "lldb/API/SBModule.h"
#include <mutex>

class LldbDriver::CompileUnit::Impl
  {
public:
  std::weak_ptr<LldbDriver::CompileUnit> myself;
  Module::Pointer module;
  lldb::SBCompileUnit compileUnit;
  std::mutex mutex;

  bool cachedTypes = false;
  std::vector<Type> types;
  };
