#pragma once
#include "Global.h"

namespace LldbDriver
{

class Process;
class Frame;

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


  friend class Process;
  };

}
