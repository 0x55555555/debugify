#include "Type.h"
#include "TypeImpl.h"
#include "FunctionMemberImpl.h"
#include "TypeMemberImpl.h"
#include "lldb/API/SBStream.h"
#include "Containers/XStringSimple.h"
#include "Containers/XStringBuilder.h"

namespace LldbDriver
{

Type::Type()
  {
  }

Type::Type(const Type &t)
    : _impl(t._impl)
  {
  }

Type::~Type()
  {
  }

Type &Type::operator=(const Type &t)
  {
  _impl = t._impl;
  return *this;
  }

const std::shared_ptr<CompileUnit> &Type::compileUnit() const
  {
  return _impl->compileUnit;
  }

std::shared_ptr<CompileUnit> &Type::compileUnit()
  {
  return _impl->compileUnit;
  }

size_t Type::size() const
  {
  return _impl->type.GetByteSize();
  }

size_t Type::memberCount() const
  {
  return _impl->type.GetNumberOfFields();
  }

TypeMember Type::member(size_t i) const
  {
  return TypeMember::Impl::make(_impl->compileUnit, _impl->type.GetFieldAtIndex(i));
  }

size_t Type::functionCount() const
  {
  return _impl->type.GetNumberOfMemberFunctions();
  }

FunctionMember Type::function(size_t i) const
  {
  return FunctionMember::Impl::make(_impl->compileUnit, _impl->type.GetMemberFunctionAtIndex(i));
  }

size_t Type::templateArgumentCount() const
  {
  return _impl->type.GetNumberOfTemplateArguments();
  }

size_t Type::directBaseClassCount() const
  {
  return _impl->type.GetNumberOfDirectBaseClasses();
  }

size_t Type::virtualBaseClassCount() const
  {
  return _impl->type.GetNumberOfVirtualBaseClasses();
  }

Type::TemplateArgumentKind Type::templateArgumentKind(size_t i) const
  {
  return (TemplateArgumentKind)_impl->type.GetTemplateArgumentKind(i);
  }

Type Type::templateArgumentType(size_t i) const
  {
  return Type::Impl::make(_impl->compileUnit, _impl->type.GetTemplateArgumentType(i));
  }

TypeMember Type::directBaseClass(size_t i) const
  {
  return TypeMember::Impl::make(_impl->compileUnit, _impl->type.GetDirectBaseClassAtIndex(i));
  }

TypeMember Type::virtualBaseClass(size_t i) const
  {
  return TypeMember::Impl::make(_impl->compileUnit, _impl->type.GetVirtualBaseClassAtIndex(i));
  }

Eks::String Type::name() const
  {
  return _impl->type.GetDisplayTypeName();
  }

Type Type::canonicalType() const
  {
  return Type::Impl::make(_impl->compileUnit, _impl->type.GetCanonicalType());
  }

Type Type::pointeeType() const
  {
  return Type::Impl::make(_impl->compileUnit, _impl->type.GetPointeeType());
  }

Type Type::dereferencedType() const
  {
  return Type::Impl::make(_impl->compileUnit, _impl->type.GetDereferencedType());
  }

Type Type::typedefType() const
  {
  return Type::Impl::make(_impl->compileUnit, _impl->type.GetTypedefedType());
  }


bool Type::isPointer() const
  {
  return _impl->type.IsPointerType();
  }

bool Type::isReference() const
  {
  return _impl->type.IsReferenceType();
  }

bool Type::isTypedef() const
  {
  return _impl->type.IsTypedefType();
  }

bool Type::isDataStructure() const
  {
  auto cls = _impl->type.GetTypeClass();
  return cls == lldb::eTypeClassClass ||
      cls == lldb::eTypeClassStruct ||
      cls == lldb::eTypeClassUnion;
  }


Eks::String Type::help() const
  {
  lldb::SBStream desc;
  _impl->type.GetDescription(desc, lldb::eDescriptionLevelVerbose);

  Eks::StringBuilder sb;
  sb << _impl->type.GetName() << " " << _impl->type.GetDisplayTypeName() << " desc: " << desc.GetData();

  return sb;
  }
}
