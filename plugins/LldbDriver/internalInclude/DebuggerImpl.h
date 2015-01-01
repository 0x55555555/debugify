#include "Debugger.h"
#include "lldb/API/SBDebugger.h"

class LldbDriver::Debugger::Impl
  {
public:
  lldb::SBDebugger debugger;
  //lldb::StateType currentState;

  //lldb::SBListener listener;
  };
