#include "Module.h"
#include "Target.h"
#include "CompileUnit.h"
#include "lldb/API/SBModule.h"
#include <mutex>

class LldbDriver::Module::Impl
  {
public:
  std::weak_ptr<LldbDriver::Module> myself;
  Target::Pointer target;
  lldb::SBModule module;
  std::mutex mutex;

  bool cachedUnits = false;
  std::vector<std::shared_ptr<LldbDriver::CompileUnit>> compileUnits;
  };
