#pragma once
#include "Global.h"
#include "Containers/XStringSimple.h"
#include <set>
#include <vector>

namespace LldbDriver
{

class CompileUnit;

/// \expose sharedpointer
class Module
  {
  SHARED_CLASS(Module);
  PIMPL_CLASS(Module, sizeof(void*) * 24);

public:
  /// \noexpose
  Module();
  ~Module();

  Eks::String path() const;

  std::set<Eks::String> files() const;

  const std::vector<std::shared_ptr<LldbDriver::CompileUnit>> &compileUnits() const;

  friend class Target;
  };

}
