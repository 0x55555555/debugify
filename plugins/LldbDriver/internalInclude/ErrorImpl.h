#include "Debugger.h"
#include "Error.h"
#include "lldb/API/SBError.h"

class LldbDriver::Error::Impl
  {
public:
  lldb::SBError error;
  };

class LldbDriver::Error::Helper
  {
public:
  static Error makeError(const lldb::SBError &err)
    {
    Error out;
    out._impl = std::make_shared<Impl>();
    out._impl->error = err;

    return out;
    }
  };
