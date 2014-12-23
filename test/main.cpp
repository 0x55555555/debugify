#include <QCoreApplication>
#include <iostream>
#include "foo.h"

void foo()
  {
  int i = 0;

  i = 1 + 3;

  std::cout << i << std::endl;
  }

int main(int argc, char *argv[])
  {
  for (int i = 0; i < argc; ++i)
    {
    Pork p;
    p.foo(argv[i]);
    }

  while(true)
    {
    foo();
    usleep(100000);
    }
  }
