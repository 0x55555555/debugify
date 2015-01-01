#include "Error.h"
#include "ErrorImpl.h"

namespace LldbDriver
{

Error::Error()
  {
  }

Error::~Error()
  {
  }

bool Error::hasError() const
  {
  return _impl->error.Fail();
  }

Eks::String Error::error() const
  {
  return _impl->error.GetCString();
  }
}
