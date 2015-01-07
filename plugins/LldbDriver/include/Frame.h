#pragma once
#include "Global.h"

namespace LldbDriver
{

class Process;

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

  friend class Process;
  };

}
