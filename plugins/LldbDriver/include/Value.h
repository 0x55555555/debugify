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

  friend class Frame;
  };

}
