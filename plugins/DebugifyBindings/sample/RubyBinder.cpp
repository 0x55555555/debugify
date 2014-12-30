#include "RubyBinder.h"
#include "RubyBindings/autogen/cpp/Debugify.h"
#include "ModuleBuilder.h"

VALUE DebugifyBindings = Qnil;

void Init_DebugifyBindings()
  {
  buildRubyModule(DebugifyBindings, Debugify::bindings());
  }
