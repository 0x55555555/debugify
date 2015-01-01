#pragma once
#include "Global.h"
#include "Containers/XStringSimple.h"

namespace LldbDriver
{

class Error
  {
public:
  Error();
  ~Error();

  class Helper;

  bool hasError() const;
  Eks::String error() const;

private:
  friend class Helper;

  class Impl;
  std::shared_ptr<Impl> _impl;
  };

}
