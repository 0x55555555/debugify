#pragma once
#include "Utilities/XStringRef.h"
#include "Crate/Type.h"
#include "RubyTypeCasters.h"

#define EKS_TYPE(type) \
  namespace Crate { namespace detail { template <> struct TypeResolver<type> { \
  static const Type *find() { static Type t; t.initialise<type>(#type, nullptr); return &t; } }; } }

EKS_TYPE(Eks::StringRef)
EKS_TYPE(Eks::String)

namespace Crate
{
namespace detail
{

template <typename T> struct TypeResolver<Eks::Vector<T>>
  {
  static const Type *find()
    {
    auto element = Crate::findType<T>;
    static Type t; t.initialise<Eks::Vector<T>>("Vector" + element->name(), nullptr);
    return &t;
    }
  };

}
}

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
      if (!canCast(b, val))
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
    if (!canCast(b, val))
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


template <typename T> class bondage::Ruby::Caster<Eks::Vector<T> &>
  {
public:
  typedef Eks::Vector<T> Result;

  static bool canCast(bondage::Ruby::Boxer *, VALUE val)
    {
    return TYPE(val) == T_ARRAY;
    }

  static Result cast(bondage::Ruby::Boxer *b, VALUE val)
    {
    if (!canCast(b, val))
      {
      throw Crate::TypeException(b->getType(val), Crate::findType<const char *>());
      }

    Result result;

    long size = RARRAY_LEN(val);
    result.resize(size);
    for (long i = 0; i < size; ++i)
      {
      VALUE el = RARRAY_PTR(val)[i];

      result[i] = Caster<T>::cast(b, el);
      }

    return result;
    }

  static void pack(bondage::Ruby::Boxer *b, VALUE *v, const Eks::Vector<T> &result)
    {
    Eks::TemporaryAllocator tmpAlloc(Eks::Core::temporaryAllocator());
    Eks::Vector<VALUE> values(&tmpAlloc);
    values.resize(result.size());

    for (size_t i = 0; i < result.size(); ++i)
      {
      Caster<T>::pack(b, &values[i], result[i]);
      }

    *v = rb_ary_new4(values.size(), values.data());
    }
  };

#undef EKS_TYPE
