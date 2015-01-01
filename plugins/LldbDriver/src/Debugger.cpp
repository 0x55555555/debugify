#include "Debugger.h"
#include "DebuggerImpl.h"
#include "Target.h"
#include "TargetImpl.h"
#include "Utilities/XAssert.h"
#include "Containers/XStringBuffer.h"
#include "lldb/API/SBTarget.h"
#include "lldb/API/SBProcess.h"
#include "lldb/API/SBListener.h"
#include "lldb/API/SBEvent.h"
#include "lldb/API/SBStream.h"
#include <iostream>

namespace LldbDriver
{

void log(const char *msg, void *)
  {
  xAssertFail();
  std::cout << msg << std::endl;
  }

Debugger::Pointer Debugger::create()
  {
  return std::make_shared<Debugger>();
  }

Debugger::Debugger()
  {
  lldb::SBDebugger::Initialize();

  _impl->debugger = lldb::SBDebugger::Create(true, log, nullptr);
  }

Debugger::~Debugger()
  {
  lldb::SBDebugger::Terminate();
  }

Target::Pointer Debugger::loadTarget(const Eks::StringRef &str)
  {
  auto target = std::make_shared<Target>();
  target->_impl->myself = target;
  target->_impl->target = _impl->debugger.CreateTarget(str.data());
  if (!target->_impl->target.IsValid())
    {
    return nullptr;
    }

  return target;
  }

}
