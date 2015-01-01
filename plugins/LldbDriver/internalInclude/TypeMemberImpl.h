#include "TypeMember.h"
#include "CompileUnit.h"
#include "lldb/API/SBType.h"

class LldbDriver::TypeMember::Impl
  {
public:
  CompileUnit::Pointer compileUnit;
  lldb::SBTypeMember member;

  static TypeMember make(const CompileUnit::Pointer &ptr, const lldb::SBTypeMember &mem)
    {
    TypeMember m;
    m._impl->compileUnit = ptr;
    m._impl->member = mem;
    return m;
    }
  };
