#pragma once
#include "Global.h"
#include <vector>

namespace LldbDriver
{

class Module;
class Type;

/// \expose sharedpointer
class CompileUnit
  {
  SHARED_CLASS(CompileUnit);
  PIMPL_CLASS(CompileUnit, sizeof(void*) * 24);

public:
  /// \noexpose
  CompileUnit();
  ~CompileUnit();

  const std::shared_ptr<Module> &module() const;

  const std::vector<Type> &types() const;

private:
  friend class Module;
  };

}
