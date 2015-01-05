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
    lldb::SBStream desc;
    ev.GetDescription(desc);

    /*std::cout << "Event: ";
    if (desc.GetData())
      {
      std::cout << desc.GetData();
      }
    std::cout << std::endl;*/
    }
  }
}
