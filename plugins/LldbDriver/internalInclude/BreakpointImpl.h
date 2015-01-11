#pragma once
#include "Breakpoint.h"
#include "Target.h"
#include "lldb/API/SBBreakpoint.h"

class LldbDriver::Breakpoint::Impl
  {
public:
  Target::Pointer target;
  lldb::SBBreakpoint breakpoint;
  };
