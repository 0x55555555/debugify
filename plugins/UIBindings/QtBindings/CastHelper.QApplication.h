#pragma once

namespace Crate
{

template <typename Derived> class CastHelper<QApplication, Derived>
  {
public:
  static bool canCast(const QApplication *)
    {
    return false;
    }
  };

}
