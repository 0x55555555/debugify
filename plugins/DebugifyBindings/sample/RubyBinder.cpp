#include "RubyBinder.h"
#include "autogen_Debugify/Debugify.h"
#include "ModuleBuilder.h"

VALUE DebugifyBindings = Qnil;

void Init_DebugifyBindings()
  {
  buildRubyModule(DebugifyBindings, Debugify::bindings());
  }
