#include "Thread.h"
#include "ThreadImpl.h"
#include "lldb/API/SBStream.h"

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

size_t Thread::id() const
  {
  return _impl->thread.GetThreadID();
  }

Eks::String Thread::name() const
  {
  auto n = _impl->thread.GetName();
  if (!n)
    {
    return "";
    }
  
    return n;
  }

bool Thread::isCurrent() const
  {
  return this == _impl->process->selectedThread().get();
  }

size_t Thread::frameCount() const
  {
  }

std::shared_ptr<Frame> Thread::frameAt(size_t i)
  {
  }

void Thread::selectFrame(const std::shared_ptr<Frame> &)
  {
  }

std::shared_ptr<Frame> Thread::selectedFrame()
  {
  }
}
