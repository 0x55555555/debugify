#include "Process.h"
#include "lldb/API/SBProcess.h"
#include "lldb/API/SBListener.h"
#include "Target.h"

class LldbDriver::Process::Impl
  {
public:
  Target::Pointer target;
  lldb::SBProcess process;
  lldb::SBListener listener;
  };
