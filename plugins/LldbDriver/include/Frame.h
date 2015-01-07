#pragma once
#include "Global.h"

namespace LldbDriver
{

class Thread;

/// \expose
class Frame
  {
  PIMPL_CLASS(Frame, sizeof(void*) * 8);

public:
  /// \noexpose
  Frame();
  /// \noexpose
  Frame(const Frame &);
  ~Frame();

  std::shared_ptr<Thread> thread();

  size_t id() const;

  Eks::String functionName() const;

  Eks::String filename() const;
  bool hasLineNumber() const;
  size_t lineNumber() const;

  bool isCurrent() const;

  friend class Thread;
  };

}
