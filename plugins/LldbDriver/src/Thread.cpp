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
  return _impl->thread.GetNumFrames();
  }

Frame Thread::frameAt(size_t index)
  {
  auto frame = _impl->thread.GetFrameAtIndex(index);

  return _impl->make(frame);
  }

void Thread::selectFrame(const Frame &f)
  {
  _impl->thread.SetSelectedFrame(f.id());
  }

Frame Thread::selectedFrame()
  {
  auto frame = _impl->thread.GetSelectedFrame();

  return _impl->make(frame);
  }
}
