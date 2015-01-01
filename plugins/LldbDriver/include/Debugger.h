#pragma once
#include "Global.h"
#include "Utilities/XStringRef.h"

namespace LldbDriver
{

class Target;

/// \expose sharedpointer
class LLDB_DRIVER_EXPORT Debugger
  {
  SHARED_CLASS(Debugger);
  PIMPL_CLASS(Target, sizeof(void*) * 2);
public:

  static Debugger::Pointer create();

  /// \noexpose
  Debugger();
  ~Debugger();

  /// Load an executable as the current target
  std::shared_ptr<Target> loadTarget(const Eks::StringRef &str);
  };

}
