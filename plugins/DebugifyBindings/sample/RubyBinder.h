#pragma once
#include <iostream>

namespace Debugify
{

/// \expose managed
class Test
  {
public:
  Test(int _i) : i(_i){ std::cout << "Create" << std::endl; }
  int i;

  void foo(Test *t) { std::cout << "fooBAZ" << t->i << std::endl; }

  Test *boink()
    {
    return this;
    }
  };

/// \expose
inline Test *foo(int a, float b)
  {
  std::cout << a << b << std::endl;
  return new Test{4};
  }

/// \expose
inline float bar(int d, long c) { std::cout << "bar" << d << " " << c << std::endl; return 55.0f; }

}

extern "C"
{
void Init_DebugifyBindings();
}

