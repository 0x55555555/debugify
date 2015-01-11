#pragma once
#include "Global.h"
#include "Containers/XStringSimple.h"

namespace LldbDriver
{

class Target;

/// \expose
class BreakpointLocation
  {
public:
  /// \noexpose
  BreakpointLocation(const char *file, size_t line, bool resolved);

  Eks::String file() const { return _file; }
  size_t line() const { return _line; }
  bool resolved() const { return _resolved; }

private:
  Eks::String _file;
  size_t _line;
  bool _resolved;
  };

/// \expose
class Breakpoint
  {
  PIMPL_CLASS(Breakpoint, 64);

public:
  Breakpoint();
  Breakpoint(const Breakpoint &);
  ~Breakpoint();

  Breakpoint &operator=(const Breakpoint &);

  const std::shared_ptr<Target> &target();

  size_t id() const;

  bool enabled() const;
  void setEnabled(bool e);

  size_t locationCount() const;
  BreakpointLocation locationAt(size_t i) const;

  friend class Target;
  };

}
