#pragma once
#include <iostream>

namespace Debugify
{

/// \expose
class Test
  {
public:
  int i;

  void foo(Test *t) { std::cout << "fooBAZ" << t->i << std::endl; }
  };

/// \expose
inline Test foo(int a, float b) { std::cout << a << b << std::endl; return Test{4}; }
/// \expose
inline float bar(int d, long c) { std::cout << "bar" << d << " " << c << std::endl; return 55.0f; }

}

extern "C"
{
void Init_DebugifyBindings();
}

