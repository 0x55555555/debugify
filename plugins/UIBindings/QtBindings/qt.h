#pragma once

#include "QWidget"
#include "QMainWindow"
#include "QApplication"
#include "bondage/RuntimeHelpers.h"

namespace Qt
{
EXPORT_DEBUGIFY const bondage::Library &bindings();
}

BONDAGE_EXPOSED_CLASS_DERIVABLE_UNMANAGED(EXPORT_DEBUGIFY, ::QWidget)
BONDAGE_EXPOSED_CLASS_DERIVED_UNMANAGED(EXPORT_DEBUGIFY, ::QMainWindow, ::QWidget, ::QWidget)
BONDAGE_EXPOSED_CLASS_UNMANAGED(EXPORT_DEBUGIFY, ::QApplication)

template <> class bondage::Ruby::Caster<QString &>
  {
public:
  typedef QString Result;

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

    QString str(QLatin1String(RSTRING_PTR(val), RSTRING_LEN(val)));
    return str;
    }

  static void pack(bondage::Ruby::Boxer *, VALUE *v, const QString &result)
    {
    *v = rb_str_new2(result.toUtf8().data());
    }
  };
