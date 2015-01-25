#include "Frame.h"
#include "FrameImpl.h"
#include "Value.h"
#include "ValueImpl.h"
#include "Math/XMathHelpers.h"
#include "Containers/XStringSimple.h"
#include "Containers/XStringBuilder.h"
#include "lldb/API/SBStream.h"
#include "lldb/API/SBLineEntry.h"
#include "Utils.h"

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
  return fileSpecAsString(file);
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

Eks::Vector<Value> Frame::arguments() const
  {
  Eks::Vector<Value> result(Eks::Core::defaultAllocator());

  auto var = _impl->frame.GetVariables(true, false, false, false, lldb::eDynamicCanRunTarget);
  result.resize(var.GetSize());

  for (size_t i = 0; i < var.GetSize(); ++i)
    {
    result[i] = Value::Impl::make(var.GetValueAtIndex(i));
    }

  return result;
  }

Eks::Vector<Value> Frame::locals() const
  {
  Eks::Vector<Value> result(Eks::Core::defaultAllocator());

  auto var = _impl->frame.GetVariables(false, true, false, false, lldb::eDynamicCanRunTarget);
  result.resize(var.GetSize());

  for (size_t i = 0; i < var.GetSize(); ++i)
    {
    result[i] = Value::Impl::make(var.GetValueAtIndex(i));
    }

  return result;
  }
}
