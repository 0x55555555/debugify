#pragma once

// Include the Ruby headers and goodies
#include "ruby.h"


extern "C"
{

// Defining a space for information and references about the module to be stored internally
VALUE MyTest = Qnil;

// Our 'test1' method.. it simply returns a value of '10' for now.
VALUE method_test1(VALUE ) {
  int x = 10;
  return INT2NUM(x);
}

// The initialization method for this module
void Init_mytest() {
  // Use this typedef to make the compiler happy when
  // calling rb_define_method()
  typedef VALUE (ruby_method)(...);

  MyTest = rb_define_module("Debugify");
  rb_define_method(MyTest, "test1", (ruby_method*)&method_test1, 0);
}

}
