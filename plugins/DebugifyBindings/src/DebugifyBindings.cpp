#include "DebugifyBindings.h"
#include "autogen/LldbDriver.h"
#include "ModuleBuilder.h"

VALUE DebugifyBindings = Qnil;

void Init_DebugifyBindings()
  {
  buildRubyModule(DebugifyBindings, LldbDriver::bindings());
  }
