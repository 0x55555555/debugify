#pragma once
#include "Global.h"

namespace LldbDriver
{

class Process;
class Frame;
class Breakpoint;
class BreakpointLocation;

/// \expose sharedpointer
class Thread
  {
  SHARED_CLASS(Thread);
  PIMPL_CLASS(Thread, sizeof(void*) * 8);

public:
  /// \noexpose
  Thread();
  ~Thread();

  std::shared_ptr<Process> process();

  size_t id() const;

  Eks::String name() const;

  bool isCurrent() const;

  size_t frameCount() const;
  Frame frameAt(size_t i);

  void selectFrame(const Frame &);
  Frame selectedFrame();

  void stepInto();
  void stepOver();
  void stepOut();

  /// \expose
  enum class StopReason
  {
      Invalid = 0,
      None,
      Trace,
      Breakpoint,
      Watchpoint,
      Signal,
      Exception,
      Exec,
      PlanComplete,
      ThreadExiting,
      Instrumentation
  };

  StopReason stopReason() const;

  /// \param[out] location
  Breakpoint stopBreakpoint(BreakpointLocation *location) const;


  friend class Process;
  };

}
