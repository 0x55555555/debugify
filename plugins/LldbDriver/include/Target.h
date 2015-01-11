#pragma once
#include "Global.h"
#include "Utilities/XStringRef.h"
#include "Utilities/XNotifier.h"

namespace LldbDriver
{

class Process;
class Module;
class Error;
class Breakpoint;

/// \expose unmanaged
X_DECLARE_NOTIFIER(BreakpointNotifier, std::function<void ()>);

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

  BreakpointNotifier *breakpointsChanged() { return &_breakpointsChanged; }

  Breakpoint addBreakpoint(const Eks::String &file, size_t line);

  size_t breakpointCount();
  Breakpoint breakpointAt(size_t index);

private:
  BreakpointNotifier _breakpointsChanged;
  friend class Debugger;
  };

}
