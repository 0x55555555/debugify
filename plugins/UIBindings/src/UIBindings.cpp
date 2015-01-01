#include "UIBindings.h"
#include "autogen/UI.h"
#include "ModuleBuilder.h"

VALUE UIBindings = Qnil;

void Init_UIBindings()
  {
  buildRubyModule(UIBindings, UI::bindings());
  }
