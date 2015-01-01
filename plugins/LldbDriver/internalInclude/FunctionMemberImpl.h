#include "FunctionMember.h"
#include "CompileUnit.h"
#include "lldb/API/SBType.h"

class LldbDriver::FunctionMember::Impl
  {
public:
  CompileUnit::Pointer compileUnit;
  lldb::SBTypeMemberFunction member;

  static FunctionMember make(const CompileUnit::Pointer &ptr, const lldb::SBTypeMemberFunction &mem)
    {
    FunctionMember m;
    m._impl->compileUnit = ptr;
    m._impl->member = mem;
    return m;
    }
  };
