#include "Thread.h"
#include "FrameImpl.h"
#include "Process.h"
#include "lldb/API/SBThread.h"

template <> struct std::hash<lldb::SBFrame>
  {
  size_t operator()(const lldb::SBFrame &) const
    {
    xAssertFail();
    return 0;
    }
  };

class LldbDriver::Thread::Impl
  {
public:
  std::weak_ptr<LldbDriver::Thread> myself;
  Process::Pointer process;
  lldb::SBThread thread;


  LldbDriver::Frame make(const lldb::SBFrame &fr)
    {
    LldbDriver::Frame frame;
    frame._impl->thread = myself.lock();
    frame._impl->frame = fr;

    return frame;
    }
  };
