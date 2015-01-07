#include "Process.h"
#include "ProcessImpl.h"
#include "Error.h"
#include "ErrorImpl.h"
#include "lldb/API/SBEvent.h"
#include "lldb/API/SBStream.h"
#include "lldb/API/SBDebugger.h"

namespace LldbDriver
{
xCompileTimeAssert((int)Process::State::Suspended == 11);

Process::Process()
  {
  }

Process::~Process()
  {
  kill();
  }

std::shared_ptr<Target> Process::target()
  {
  return _impl->target;
  }

size_t Process::processID() const
  {
  return _impl->process.GetProcessID();
  }

Process::State Process::currentState() const
  {
  auto state = _impl->process.GetState();

  return (Process::State)state;
  }

Eks::String Process::getStateString(Process::State s)
  {
  return lldb::SBDebugger::StateAsCString((lldb::StateType)s);
  }

size_t Process::getOutput(Process::OutputType type, char *data, size_t inputSize)
  {
  if (type == OutputType::Error)
    {
    return _impl->process.GetSTDERR(data, inputSize);
    }
  else if (type == OutputType::Output)
    {
    return _impl->process.GetSTDOUT(data, inputSize);
    }

  return 0;
  }

Error Process::kill()
  {
  auto err = _impl->process.Kill();
  return Error::Helper::makeError(err);
  }

Error Process::pauseExecution()
  {
  auto err = _impl->process.Stop();
  return Error::Helper::makeError(err);
  }

Error Process::continueExecution()
  {
  auto err = _impl->process.Continue();
  return Error::Helper::makeError(err);
  }

void Process::processEvents()
  {
  lldb::SBEvent ev;
  while (_impl->listener.GetNextEvent(ev))
    {
    }
  }

size_t Process::threadCount()
  {
  return _impl->process.GetNumThreads();
  }

std::shared_ptr<Thread> Process::threadAt(size_t index)
  {
  auto thread = _impl->process.GetThreadAtIndex(index);

  return _impl->wrapThread(thread);
  }

void Process::selectThread(const std::shared_ptr<Thread> &t)
  {
  if (!t)
    {
    return;
    }

  _impl->process.SetSelectedThread(t->_impl->thread);
  }

std::shared_ptr<Thread> Process::selectedThread()
  {
  return _impl->wrapThread(_impl->process.GetSelectedThread());
  }
}
