#pragma once
#include "Global.h"
#include "Utilities/XStringRef.h"

namespace LldbDriver
{

class Process;
class Module;
class Error;

/// \expose sharedpointer
class Target
  {
  SHARED_CLASS(Target);
  PIMPL_CLASS(Target, 64);

public:
  /// \noexpose
  Target();
  ~Target();

  Eks::String path() const;

  std::shared_ptr<Process> launch(
    const Eks::Vector<Eks::StringRef> &args,
    const Eks::Vector<Eks::StringRef> &env,
    Error &err);

  size_t moduleCount();
  std::shared_ptr<Module> moduleAt(size_t index);

private:
  friend class Debugger;
  };

}
