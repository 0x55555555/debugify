#pragma once
#include "Global.h"
#include "Containers/XVector.h"

namespace LldbDriver
{

class Value;
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
  bool inlined() const;

  bool isCurrent() const;

  Eks::Vector<Value> arguments() const;
  Eks::Vector<Value> locals() const;

  Value evaluate(const char *e) const;

  friend class Thread;
  };

}
