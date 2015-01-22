#include "Thread.h"
#include "ThreadImpl.h"
#include "lldb/API/SBStream.h"
#include "Target.h"
#include "Breakpoint.h"

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

void Thread::stepInto()
  {
  _impl->thread.StepInto();
  }

void Thread::stepOver()
  {
  _impl->thread.StepOver();
  }

void Thread::stepOut()
  {
  _impl->thread.StepOut();
  }

Thread::StopReason Thread::stopReason() const
  {
  return (StopReason)_impl->thread.GetStopReason();
  }

/// \param[out] location
Breakpoint Thread::stopBreakpoint(BreakpointLocation *location) const
  {
  auto t = _impl->process->target();
  if (!t)
    {
    return Breakpoint();
    }

  auto breakpointId = _impl->thread.GetStopReasonDataAtIndex(0);
  int breakpointLocationId = _impl->thread.GetStopReasonDataAtIndex(1);

  Breakpoint result;
  for (size_t i = 0; i < t->breakpointCount(); ++i)
    {
    auto bk = t->breakpointAt(i);
    if (bk.id() == breakpointId)
      {
      result = bk;
      break;
      }
    }
    
  for (size_t i = 0; i < result.locationCount(); ++i)
  {
    auto loc = result.locationAt(i);
    if (loc.id() == breakpointLocationId)
      {
      *location = loc;
      break;
      }

    }
  return result;
  }
}
