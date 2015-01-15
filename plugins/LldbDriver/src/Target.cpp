#include "Target.h"
#include "TargetImpl.h"
#include "Process.h"
#include "ProcessImpl.h"
#include "ErrorImpl.h"
#include <iostream>
#include "lldb/API/SBBreakpointLocation.h"
#include "Utils.h"

namespace LldbDriver
{

Target::Target()
  {
  }

Target::~Target()
  {
  }

Eks::String Target::path() const
  {
  Eks::StringBuilder sb;
  sb << _impl->target.GetExecutable().GetDirectory() << "/" << _impl->target.GetExecutable().GetFilename();

  return sb;
  }


Process::Pointer Target::launch(
    const Eks::Vector<Eks::StringRef> &args,
    const Eks::Vector<Eks::StringRef> &env,
    Error &err)
  {
  lldb::SBError error;

  auto process = std::make_shared<Process>();

  auto bake = [](auto &input, auto &storage, auto &pointers)
    {
    xForeach(const auto &i, input)
      {
      storage << Eks::String();
      i.toString(storage.back());
      }

    xForeach(const auto &i, storage)
      {
      pointers << i.data();
      }

    pointers << nullptr;
    };

  Eks::TemporaryAllocator alloc(Eks::Core::temporaryAllocator());
  Eks::Vector<Eks::String> argsStorage(&alloc);
  Eks::Vector<const char *> argsData(&alloc);
  bake(args, argsStorage, argsData);

  Eks::Vector<Eks::String> envStorage(&alloc);
  Eks::Vector<const char *> envData(&alloc);
  bake(env, envStorage, envData);

  process->_impl->myself = process;
  process->_impl->target = _impl->myself.lock();
  process->_impl->listener = lldb::SBListener("ProcessListener");
  process->_impl->process = _impl->target.Launch(
        process->_impl->listener,
        argsData.data(),
        envData.data(),
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        lldb::eLaunchFlagNone,
        false,
        error);

  err = Error::Helper::makeError(error);
  return process;
  }

size_t Target::moduleCount()
  {
  _impl->cacheModules();
  return _impl->modules.size();
  }

std::shared_ptr<Module> Target::moduleAt(size_t index)
  {
  _impl->cacheModules();
  return _impl->modules[index];
  }

bool Target::findBreakpoint(const Eks::String &file, size_t line, Breakpoint *outBrk, BreakpointLocation *outLoc)
  {
  auto count = breakpointCount();
  for (size_t i = 0; i < count; ++i)
    {
    auto brk = breakpointAt(i);

    if (brk.findLocation(file, line, outLoc))
      {
      *outBrk = brk;
      return true;
      }
    }
  return false;
  }

Breakpoint Target::addBreakpoint(const Eks::String &file, size_t line)
  {
  auto br = _impl->make(_impl->target.BreakpointCreateByLocation(file.data(), line));

  _breakpointsChanged();
  return br;
  }

bool Target::removeBreakpoint(const Breakpoint &brk)
  {
  auto res = _impl->target.BreakpointDelete(brk._impl->breakpoint.GetID());

  _breakpointsChanged();
  return res;
  }

size_t Target::breakpointCount()
  {
  return _impl->target.GetNumBreakpoints();
  }

Breakpoint Target::breakpointAt(size_t index)
  {
  return _impl->make(_impl->target.GetBreakpointAtIndex(index));
  }
}
