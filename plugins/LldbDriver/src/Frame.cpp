#include "Frame.h"
#include "FrameImpl.h"
#include "Math/XMathHelpers.h"
#include "Containers/XStringSimple.h"
#include "Containers/XStringBuilder.h"
#include "lldb/API/SBStream.h"
#include "lldb/API/SBLineEntry.h"

namespace LldbDriver
{

Frame::Frame()
  {
  }

Frame::Frame(const Frame &f) : _impl(f._impl)
  {
  }

Frame::~Frame()
  {
  }

std::shared_ptr<Thread> Frame::thread()
  {
  return _impl->thread;
  }

size_t Frame::id() const
  {
  return _impl->frame.GetFrameID();
  }

Eks::String Frame::functionName() const
  {
  auto n = _impl->frame.GetFunctionName();
  if (!n)
    {
    return "";
    }

  return n;
  }

Eks::String Frame::filename() const
  {
  auto file = _impl->frame.GetLineEntry().GetFileSpec();
  file.ResolveExecutableLocation();

  Eks::StringBuilder sb;
  if (file.GetDirectory() && file.GetFilename())
    {
    sb << file.GetDirectory() << "/" << file.GetFilename();
    }

  return sb;
  }

bool Frame::hasLineNumber() const
  {
  return _impl->frame.GetLineEntry().GetLine() != Eks::maxFor<uint32_t>();
  }

size_t Frame::lineNumber() const
  {
  return _impl->frame.GetLineEntry().GetLine();
  }

bool Frame::isCurrent() const
  {
  return _impl->frame == _impl->thread->selectedFrame()._impl->frame;
  }
}
