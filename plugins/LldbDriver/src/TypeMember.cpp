#include "TypeMember.h"
#include "TypeMemberImpl.h"
#include "TypeImpl.h"
#include "Containers/XStringSimple.h"

namespace LldbDriver
{

TypeMember::TypeMember()
  {
  }

TypeMember::TypeMember(const TypeMember &t)
    : _impl(t._impl)
  {
  }

TypeMember::~TypeMember()
  {
  }

TypeMember &TypeMember::operator=(const TypeMember &t)
  {
  _impl = t._impl;
  return *this;
  }

Eks::String TypeMember::name() const
  {
  auto name = _impl->member.GetName();
  if (!name)
    {
    return Eks::String();
    }
  return name;
  }

Type TypeMember::type() const
  {
  return Type::Impl::make(_impl->compileUnit, _impl->member.GetType());
  }

size_t TypeMember::byteOffset() const
  {
  return _impl->member.GetOffsetInBytes();
  }

size_t TypeMember::bitOffset() const
  {
  return _impl->member.GetOffsetInBits();
  }

}
