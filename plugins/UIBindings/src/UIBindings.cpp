#include "UIBindings.h"
#include "autogen/UI.h"
#include "Qt.h"
#include "ModuleBuilder.h"

VALUE QtBindings = Qnil;
VALUE UIBindings = Qnil;

void Init_UIBindings()
  {
  buildRubyModule(QtBindings, Qt::bindings());
  buildRubyModule(UIBindings, UI::bindings());
  }
