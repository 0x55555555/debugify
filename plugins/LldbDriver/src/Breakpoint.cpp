#include "Breakpoint.h"
#include "BreakpointImpl.h"
#include "lldb/API/SBBreakpointLocation.h"
#include "lldb/API/SBAddress.h"
#include "Utils.h"

namespace LldbDriver
{

BreakpointLocation::BreakpointLocation(const char *file, size_t line, bool resolved)
    : _file(file),
      _line(line),
      _resolved(resolved)
  {
  }

Breakpoint::Breakpoint()
  {
  }

Breakpoint::Breakpoint(const Breakpoint &t)
    : _impl(t._impl)
  {
  }

Breakpoint::~Breakpoint()
  {
  }

Breakpoint &Breakpoint::operator=(const Breakpoint &t)
  {
  _impl = t._impl;
  return *this;
  }

const std::shared_ptr<Target> &Breakpoint::target()
  {
  return _impl->target;
  }

size_t Breakpoint::id() const
  {
  return _impl->breakpoint.GetID();
  }

bool Breakpoint::enabled() const
  {
  return _impl->breakpoint.IsEnabled();
  }

void Breakpoint::setEnabled(bool e)
  {
  return _impl->breakpoint.SetEnabled(e);
  }

size_t Breakpoint::locationCount() const
  {
  return _impl->breakpoint.GetNumLocations() + _impl->breakpoint.GetNumResolvedLocations();
  }

BreakpointLocation Breakpoint::locationAt(size_t i) const
  {
  auto loc = _impl->breakpoint.GetLocationAtIndex(i);

  auto lineSpec = loc.GetAddress().GetLineEntry();
  auto file = loc.GetAddress().GetLineEntry().GetFileSpec();

  return BreakpointLocation{ fileSpecAsString(file).data(), lineSpec.GetLine(), loc.IsResolved() };
  }

}
