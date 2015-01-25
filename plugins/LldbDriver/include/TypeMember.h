#pragma once
#include "Global.h"

namespace LldbDriver
{

class Type;

/// \expose
class TypeMember
  {
  PIMPL_CLASS(TypeMember, sizeof(void*)*16);

public:
  TypeMember();
  TypeMember(const TypeMember &);
  ~TypeMember();

  TypeMember &operator=(const TypeMember &);

  Eks::String name() const;

  Type type() const;

  size_t byteOffset() const;
  size_t bitOffset() const;

private:
  friend class Type;
  };

}
