#pragma once
#include "Global.h"

namespace LldbDriver
{

class CompileUnit;
class TypeMember;
class FunctionMember;

/// \expose
class Type
  {
  PIMPL_CLASS(Type, 64);

public:
  Type();
  Type(const Type &);
  ~Type();

  Type &operator=(const Type &);

  const std::shared_ptr<CompileUnit> &compileUnit() const;
  std::shared_ptr<CompileUnit> &compileUnit();

  bool getLocation(Eks::String &file, size_t &line) const;

  size_t size() const;

  size_t memberCount() const;
  TypeMember member(size_t i) const;

  size_t functionCount() const;
  FunctionMember function(size_t i) const;

  enum TemplateArgumentKind
    {
    KindNull,
    KindType,
    KindDeclaration,
    KindIntegral,
    KindTemplate,
    KindTemplateExpansion,
    KindExpression,
    KindPack
    };

  size_t templateArgumentCount() const;
  TemplateArgumentKind templateArgumentKind(size_t i) const;
  Type templateArgumentType(size_t i) const;

  size_t directBaseClassCount() const;
  TypeMember directBaseClass(size_t i) const;

  size_t virtualBaseClassCount() const;
  TypeMember virtualBaseClass(size_t i) const;

  Eks::String name() const;

  Type canonicalType() const;
  Type pointeeType() const;
  Type dereferencedType() const;
  Type typedefType() const;

  bool isPointer() const;
  bool isReference() const;
  bool isTypedef() const;

  bool isDataStructure() const;

  Eks::String help() const;

private:
  friend class CompileUnit;
  friend class TypeMember;
  friend class Value;
  friend class FunctionMember;
  };

}
