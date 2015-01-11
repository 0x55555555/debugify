#pragma once
#include "lldb/API/SBFileSpec.h"

namespace LldbDriver
{

inline Eks::StringBuilder fileSpecAsString(lldb::SBFileSpec file)
  {
  file.ResolveExecutableLocation();

  Eks::StringBuilder sb;
  if (file.GetDirectory() && file.GetFilename())
    {
    sb << file.GetDirectory() << "/" << file.GetFilename();
    }

  return std::move(sb);
  }

}
