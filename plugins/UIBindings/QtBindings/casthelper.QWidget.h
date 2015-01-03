#pragma once

namespace Crate
{

template <typename Derived> class CastHelper<QWidget, Derived>
  {
public:
  static bool canCast(const QWidget *ptr)
    {
    return qobject_cast<const Derived*>(ptr) != nullptr;
    }
  };

}
