#include "RubyBinder.h"
#include "autogen/LldbDriver.h"
#include "ModuleBuilder.h"

VALUE DebugifyBindings = Qnil;

void Init_DebugifyBindings()
  {
  static Eks::Core eks;
  buildRubyModule(DebugifyBindings, LldbDriver::bindings());
  }
