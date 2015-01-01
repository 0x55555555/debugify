#pragma once
#include "Global.h"

namespace LldbDriver
{

class Type;

/// \expose
class FunctionMember
  {
  PIMPL_CLASS(FunctionMember, 64);

public:
  FunctionMember();
  FunctionMember(const FunctionMember &);
  ~FunctionMember();

  FunctionMember &operator=(const FunctionMember &);

  Eks::String name() const;

  Type returnType() const;

  size_t argumentCount() const;
  Type argument(size_t i) const;

private:
  friend class Type;
  };

}
