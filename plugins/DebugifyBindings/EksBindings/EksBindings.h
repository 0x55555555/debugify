#pragma once
#include "Utilities/XStringRef.h"
#include "Crate/Type.h"
#include "RubyTypeCasters.h"

#define EKS_TYPE(type) \
  template <> struct Crate::detail::TypeResolver<type> { \
  static const Crate::Type *find() { static Crate::Type t; t.initialise<type>(#type, nullptr); return &t; } };

EKS_TYPE(Eks::StringRef)
EKS_TYPE(Eks::String)

template <> class bondage::Ruby::Caster<Eks::String &>
  {
public:
  typedef Eks::String Result;

  static bool canCast(bondage::Ruby::Boxer *, VALUE val)
    {
    return TYPE(val) == T_STRING;
    }

  static Result cast(bondage::Ruby::Boxer *b, VALUE val)
    {
    if (TYPE(val) != T_STRING)
      {
      throw Crate::TypeException(b->getType(val), Crate::findType<const char *>());
      }

    Eks::String str(RSTRING_PTR(val), RSTRING_LEN(val));
    return str;
    }

  static void pack(bondage::Ruby::Boxer *, VALUE *v, const Eks::String &result)
    {
    *v = rb_str_new2(result.data());
    }
  };

template <> class bondage::Ruby::Caster<Eks::StringRef &>
  {
public:
  typedef Eks::String Result;

  static bool canCast(bondage::Ruby::Boxer *, VALUE val)
    {
    return TYPE(val) == T_STRING;
    }

  static Result cast(bondage::Ruby::Boxer *b, VALUE val)
    {
    if (TYPE(val) != T_STRING)
      {
      throw Crate::TypeException(b->getType(val), Crate::findType<const char *>());
      }

    Eks::String str(RSTRING_PTR(val), RSTRING_LEN(val));
    return str;
    }

  static void pack(bondage::Ruby::Boxer *, VALUE *v, const Eks::StringRef &result)
    {
    Eks::StringBuilder bld = result.toBuilder();
    *v = rb_str_new2(bld.data());
    }
  };

#undef EKS_TYPE
