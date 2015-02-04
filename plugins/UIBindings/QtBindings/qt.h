#pragma once

#include "QWidget"
#include "QDialog"
#include "QMainWindow"
#include "QTextEdit"
#include "QApplication"
#include "QToolBar"
#include "QMenu"
#include "QAction"
#include "QDockWidget"
#include "QVariant"
#include "bondage/RuntimeHelpers.h"
#include "Memory/XTemporaryAllocator.h"
#include "Containers/XVector.h"
#include "XCore.h"

namespace Qt
{
EXPORT_DEBUGIFY const bondage::Library &bindings();
}

BONDAGE_EXPOSED_CLASS_DERIVABLE_UNMANAGED(EXPORT_DEBUGIFY, ::QAction)
BONDAGE_EXPOSED_CLASS_DERIVABLE_UNMANAGED(EXPORT_DEBUGIFY, ::QWidget)
BONDAGE_EXPOSED_CLASS_DERIVED_UNMANAGED(EXPORT_DEBUGIFY, ::QDialog, ::QWidget, ::QWidget)
BONDAGE_EXPOSED_CLASS_DERIVED_UNMANAGED(EXPORT_DEBUGIFY, ::QMainWindow, ::QWidget, ::QWidget)
BONDAGE_EXPOSED_CLASS_DERIVED_UNMANAGED(EXPORT_DEBUGIFY, ::QDockWidget, ::QWidget, ::QWidget)
BONDAGE_EXPOSED_CLASS_DERIVED_UNMANAGED(EXPORT_DEBUGIFY, ::QFrame, ::QWidget, ::QWidget)
BONDAGE_EXPOSED_CLASS_DERIVED_UNMANAGED(EXPORT_DEBUGIFY, ::QAbstractScrollArea, ::QFrame, ::QWidget)
BONDAGE_EXPOSED_CLASS_DERIVED_UNMANAGED(EXPORT_DEBUGIFY, ::QTextEdit, ::QAbstractScrollArea, ::QWidget)
BONDAGE_EXPOSED_CLASS_DERIVED_UNMANAGED(EXPORT_DEBUGIFY, ::QToolBar, ::QWidget, ::QWidget)
BONDAGE_EXPOSED_CLASS_DERIVED_UNMANAGED(EXPORT_DEBUGIFY, ::QMenu, ::QWidget, ::QWidget)
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
    if (!canCast(b, val))
      {
      throw Crate::TypeException(Crate::findType<const char *>(), b->getType(val));
      }

    QString str(QLatin1String(RSTRING_PTR(val), RSTRING_LEN(val)));
    return str;
    }

  static void pack(bondage::Ruby::Boxer *, VALUE *v, const QString &result)
    {
    *v = rb_str_new2(result.toUtf8().data());
    }
  };

template <> class bondage::Ruby::Caster<QVariant &>
  {
public:
  typedef QVariant Result;

  static bool canCast(bondage::Ruby::Boxer *, VALUE)
    {
    return true;
    }

  static Result cast(bondage::Ruby::Boxer *b, VALUE val)
    {
    if (TYPE(val) == T_NIL)
      {
      return QVariant();
      }
    else if (TYPE(val) == T_STRING)
      {
      return Caster<QString &>::cast(b, val);
      }
    else if(TYPE(val) == T_FIXNUM)
      {
      return Caster<double>::cast(b, val);
      }
    else if(TYPE(val) == T_ARRAY)
      {
      QList<QVariant> v;

      long size = RARRAY_LEN(val);
      for (long i = 0; i < size; ++i)
        {
        VALUE el = RARRAY_PTR(val)[i];

        v << Caster<QVariant>::cast(b, el);
        }
      return v;
      }
    else if(TYPE(val) == T_TRUE || TYPE(val) == T_FALSE)
      {
      return Caster<bool>::cast(b, val);
      }
    else
      {
      throw std::runtime_error("unknown variant type");
      }
    }

  static void pack(bondage::Ruby::Boxer *b, VALUE *v, const QVariant &result)
    {
    if (result.isNull())
      {
      *v = Qnil;
      }
    else if (result.type() == QVariant::String)
      {
      Caster<QString &>::pack(b, v, result.toString());
      }
    else if (result.type() == QVariant::Bool)
      {
      Caster<double>::pack(b, v, result.toBool());
      }
    else if (result.canConvert<double>())
      {
      Caster<double>::pack(b, v, result.toDouble());
      }
    else if (result.canConvert<QList<QVariant>>())
      {
      auto val = result.value<QList<QVariant>>();

      Eks::TemporaryAllocator tmpAlloc(Eks::Core::temporaryAllocator());
      Eks::Vector<VALUE> values(&tmpAlloc);
      values.resize(val.size());

      for (int i = 0; i < val.size(); ++i)
        {
        Caster<QVariant>::pack(b, &values[i], val[i]);
        }

      *v = rb_ary_new4(values.size(), values.data());
      }
    else
      {
      throw std::runtime_error("unknown variant type");
      }
    }
  };
