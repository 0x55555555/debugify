#include "FunctionMember.h"
#include "FunctionMemberImpl.h"
#include "TypeImpl.h"
#include "Containers/XStringSimple.h"

namespace LldbDriver
{

FunctionMember::FunctionMember()
  {
  }

FunctionMember::FunctionMember(const FunctionMember &t)
    : _impl(t._impl)
  {
  }

FunctionMember::~FunctionMember()
 {
  }

FunctionMember &FunctionMember::operator=(const FunctionMember &t)
  {
  _impl = t._impl;
  return *this;
  }

Eks::String FunctionMember::name() const
  {
  return _impl->member.GetName();
  }

Type FunctionMember::returnType() const
  {
  return Type::Impl::make(_impl->compileUnit, _impl->member.GetReturnType());
  }

size_t FunctionMember::argumentCount() const
  {
  return _impl->member.GetNumberOfArguments();
  }

Type FunctionMember::argument(size_t i) const
  {
  return Type::Impl::make(_impl->compileUnit, _impl->member.GetArgumentTypeAtIndex(i));
  }
}
