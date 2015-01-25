#include "Value.h"
#include "ValueImpl.h"
#include "FrameImpl.h"
#include "TypeImpl.h"
#include "Containers/XStringSimple.h"

namespace LldbDriver
{

Value::Value()
  {
  }

Value::Value(const Value &f) : _impl(f._impl)
  {
  }

Value &Value::operator=(const Value &v)
  {
  _impl = v._impl;
  return *this;
  }

Value::~Value()
  {
  }

Eks::String Value::name() const
  {
  auto n = _impl->value.GetName();
  if (!n)
    {
    return Eks::String();
    }
  return n;
  }

Eks::String Value::value() const
  {
  auto n = _impl->value.GetValue();
  if (!n)
    {
    return Eks::String();
    }
  return n;
  }

bool Value::inScope() const
  {
  return _impl->value.IsInScope();
  }

bool Value::changed() const
  {
  return _impl->value.GetValueDidChange();
  }

Type Value::type() const
  {
  auto t = _impl->value.GetType();

  return Type::Impl::make(nullptr, t);
  }
}
