// Autogenerated - do not change.
#include "../../UIBindings/src/autogen/UI.h"
#include "bondage/RuntimeHelpersImpl.h"
#include "utility"
#include "tuple"



using namespace UI;


bondage::Library g_bondage_library_UI(
  "UI",
  nullptr,
  0);
namespace UI
{
const bondage::Library &bindings()
{
  return g_bondage_library_UI;
}
}


// Exposing class ::UI::Application
::UI::Application * UI_Application_Application_overload0()
{
  auto result = bondage::WrappedClassHelper< ::UI::Application >::create();
  return result;
}

struct UI_Application_Application_overload0_t : Reflect::FunctionCall<Reflect::FunctionSignature< ::UI::Application *(*)() >, &UI_Application_Application_overload0, bondage::FunctionCaller> { };

const bondage::Function UI_Application_methods[] = {
  bondage::FunctionBuilder::build<
    UI_Application_Application_overload0_t
    >("Application")
};


BONDAGE_IMPLEMENT_EXPOSED_CLASS(
  UI_Application,
  g_bondage_library_UI,
  ::UI,
  Application,
  void,
  UI_Application_methods,
  1);



