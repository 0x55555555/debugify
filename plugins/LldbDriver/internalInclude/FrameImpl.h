#include "Frame.h"
#include "lldb/API/SBFrame.h"
#include "Thread.h"

class LldbDriver::Frame::Impl
  {
public:
  Thread::Pointer thread;
  lldb::SBFrame frame;
  };
