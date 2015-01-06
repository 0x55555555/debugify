#include "Thread.h"
#include "ThreadImpl.h"

namespace LldbDriver
{

Thread::Thread()
  {
  }

Thread::~Thread()
  {
  }

std::shared_ptr<Process> Thread::process()
  {
  return _impl->process;
  }

size_t Thread::threadID() const
  {
  return _impl->thread.GetThreadID();
  }
}
