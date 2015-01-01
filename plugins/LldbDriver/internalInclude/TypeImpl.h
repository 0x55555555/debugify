#pragma once
#include "Type.h"
#include "CompileUnit.h"
#include "lldb/API/SBType.h"

class LldbDriver::Type::Impl
  {
public:
  CompileUnit::Pointer compileUnit;
  lldb::SBType type;

  static Type make(const CompileUnit::Pointer &ptr, lldb::SBType type)
    {
    Type t;
    t._impl->type = type;
    t._impl->compileUnit = ptr;

    return t;
    }
  };
