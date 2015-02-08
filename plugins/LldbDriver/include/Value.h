#pragma once
#include "Global.h"

namespace LldbDriver
{

class Type;

/// \expose
class Value
  {
  PIMPL_CLASS(Value, sizeof(void*) * 8);

public:
  /// \noexpose
  Value();
  /// \noexpose
  Value(const Value &v);
  Value &operator=(const Value &v);
  ~Value();

  Eks::String name() const;
  Eks::String value() const;

  bool inScope() const;
  bool changed() const;

  Type type() const;

  bool isPointerType() const;
  Value address() const;
  Value dereference() const;

  bool mightHaveChildren() const;
  size_t childCount() const;
  Value childAt(size_t i) const;

  Value evaluate(const char *name, const char *) const;
  Value evaluateChild(const char *) const;

  friend class Frame;
  friend class Thread;
  };

}
