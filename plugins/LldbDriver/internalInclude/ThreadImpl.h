#include "Thread.h"
#include "lldb/API/SBThread.h"
#include "Process.h"

class LldbDriver::Thread::Impl
  {
public:
  std::weak_ptr<LldbDriver::Thread> myself;
  Process::Pointer process;
  lldb::SBThread thread;
  };
