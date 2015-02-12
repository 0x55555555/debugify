#include <QCoreApplication>
#include <iostream>
#include <unistd.h>
#include <future>
#include <array>
#include <QVector>
#include "foo.h"

void foo()
  {
  //auto test = std::make_unique<Pork>();

  auto vec = std::vector<float>({1, 2, 3, 4, 5});

  //std::map<int, float> f;
  //f[1] = 1.0f;
  //f[3] = 2.0f;
  //f[2] = 5.0f;

  //std::array<double, 10> arr;
  //arr[0] = 4;

  //*i = 1 + 3;

  //std::cout << *i << std::endl;
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
    threads.push_back(std::make_unique<std::thread>([i]()
      {
      while(true)
        {
        foo();
        usleep(100000);
        }
      }));
    }


  for(auto &t : threads)
    {
    t->join();
    }
  }
