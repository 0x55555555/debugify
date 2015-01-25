#include "Value.h"
#include "lldb/API/SBValue.h"
#include "Thread.h"

class LldbDriver::Value::Impl
  {
public:
  lldb::SBValue value;

  static Value make(const lldb::SBValue &val)
    {
    Value v;

    v._impl->value = val;

    return v;
    }
  };
