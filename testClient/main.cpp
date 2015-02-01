#include <QCoreApplication>
#include <iostream>
#include <unistd.h>
#include <future>
#include <QVector>
#include "foo.h"

void foo(int *i)
  {
  if (!i)
    {
    throw std::runtime_error("SHIT SHIT SHIT");
    }
  *i = 0;
  auto test = std::make_unique<Pork>();

  *i = 1 + 3;

  std::cout << *i << std::endl;
  std::cerr << 5 << std::endl;
  }

int main(int argc, char *argv[])
  {
  QVector<int> test;

  for (int i = 0; i < argc; ++i)
    {
    Pork p;
    p.foo(argv[i]);
    test.push_back(i);
    }

  std::vector<std::unique_ptr<std::thread>> threads;

  for (int i = 0; i < 10; ++i)
    {
    threads.push_back(std::make_unique<std::thread>([]()
      {
      while(true)
        {
        foo(nullptr);
        usleep(100000);
        }
      }));
    }


  for(auto &t : threads)
    {
    t->join();
    }
  }
