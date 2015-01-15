#include "foo.h"

void sample()
  {
  Pork p;

  int i = 4 + (size_t)&p;

  p.foo("test" + std::to_string(i));
  }
