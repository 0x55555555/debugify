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

bool Value::isPointerType() const
  {
  return _impl->value.TypeIsPointerType();
  }

Value Value::address() const
  {
  return Value::Impl::make(_impl->value.AddressOf());
  }

Value Value::dereference() const
  {
  return Value::Impl::make(_impl->value.Dereference());
  }

bool Value::mightHaveChildren() const
  {
  return _impl->value.MightHaveChildren();
  }

size_t Value::childCount() const
  {
  return _impl->value.GetNumChildren();
  }

Value Value::childAt(size_t i) const
  {
  return Value::Impl::make(_impl->value.GetChildAtIndex(i, lldb::eNoDynamicValues, false));
  }

Value Value::evaluate(const char *name, const char *exp) const
  {
  return Value::Impl::make(_impl->value.CreateValueFromExpression(name, exp));
  }

Value Value::evaluateChild(const char *path) const
  {
  return Value::Impl::make(_impl->value.GetValueForExpressionPath(path));
  }

}
