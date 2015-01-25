#include "Qt.h"
#include "bondage/RuntimeHelpersImpl.h"

#define BONDAGE_MANUAL_LIBRARY_SPECIFIC(name, functions, functionCount) \
  bondage::Library g_bondage_library_##name (#name, functions, functionCount); \
  namespace name { const bondage::Library &bindings() { \
    return g_bondage_library_##name; } }

#define BONDAGE_MANUAL_LIBRARY(name, functions) BONDAGE_MANUAL_LIBRARY_SPECIFIC(name, functions, sizeof(functions)/sizeof(functions[0]))
#define BONDAGE_MANUAL_LIBRARY_NO_FUNCTIONS(name) BONDAGE_MANUAL_LIBRARY_SPECIFIC(name, nullptr, 0)


#define BONDAGE_MANUAL_CLASS_SPECIFIC(ns, codeNs, cls, parent, functions, functionCount) \
  BONDAGE_IMPLEMENT_EXPOSED_CLASS(ns##_##cls, g_bondage_library_##ns, codeNs, cls, parent, functions, functionCount);

#define BONDAGE_MANUAL_CLASS(ns, codeNs, cls, parent) BONDAGE_MANUAL_CLASS_SPECIFIC(ns, codeNs, cls, parent, ns##_##cls##_methods, sizeof(ns##_##cls##_methods)/sizeof(ns##_##cls##_methods[0]))

#define BONDAGE_MANUAL_CLASS_FUNCTIONS_BEGIN(ns, cls) const bondage::Function ns##_##cls##_methods[] = {
#define BONDAGE_MANUAL_CLASS_FUNCTIONS_END(ns, cls) };

#define BONDAGE_MANUAL_CLASS_METHOD(cls, fn) bondage::FunctionBuilder::build< \
  Reflect::FunctionCall<Reflect::FunctionSignature<decltype(&cls::fn)>, &cls::fn, bondage::FunctionCaller> \
  >(#fn)

#define BONDAGE_MANUAL_CLASS_INJECTED_METHOD(fn, name) bondage::FunctionBuilder::build< \
  Reflect::FunctionCall<Reflect::FunctionSignature< decltype(&fn) >, &fn, Reflect::MethodInjectorBuilder<bondage::FunctionCaller>>>(#name)

#define BONDAGE_NULL_NAMESPACE
#define BONDAGE_NULL_PARENT void



BONDAGE_MANUAL_LIBRARY_NO_FUNCTIONS(Qt)

BONDAGE_MANUAL_CLASS_FUNCTIONS_BEGIN(Qt, QAction)
  BONDAGE_MANUAL_CLASS_METHOD(QAction, setText),
BONDAGE_MANUAL_CLASS_FUNCTIONS_END(Qt, QAction)

BONDAGE_MANUAL_CLASS(Qt, BONDAGE_NULL_NAMESPACE, QAction, BONDAGE_NULL_PARENT)

BONDAGE_MANUAL_CLASS_FUNCTIONS_BEGIN(Qt, QWidget)
  BONDAGE_MANUAL_CLASS_METHOD(QWidget, show),
  BONDAGE_MANUAL_CLASS_METHOD(QWidget, hide)
BONDAGE_MANUAL_CLASS_FUNCTIONS_END(Qt, QWidget)


BONDAGE_MANUAL_CLASS(Qt, BONDAGE_NULL_NAMESPACE, QWidget, BONDAGE_NULL_PARENT)

BONDAGE_MANUAL_CLASS_FUNCTIONS_BEGIN(Qt, QMainWindow)
BONDAGE_MANUAL_CLASS_FUNCTIONS_END(Qt, QMainWindow)
BONDAGE_MANUAL_CLASS(Qt, BONDAGE_NULL_NAMESPACE, QMainWindow, QWidget)

BONDAGE_MANUAL_CLASS_FUNCTIONS_BEGIN(Qt, QFrame)
BONDAGE_MANUAL_CLASS_FUNCTIONS_END(Qt, QFrame)
BONDAGE_MANUAL_CLASS(Qt, BONDAGE_NULL_NAMESPACE, QFrame, QWidget)

BONDAGE_MANUAL_CLASS_FUNCTIONS_BEGIN(Qt, QAbstractScrollArea)
BONDAGE_MANUAL_CLASS_FUNCTIONS_END(Qt, QAbstractScrollArea)
BONDAGE_MANUAL_CLASS(Qt, BONDAGE_NULL_NAMESPACE, QAbstractScrollArea, QFrame)

BONDAGE_MANUAL_CLASS_FUNCTIONS_BEGIN(Qt, QTextEdit)
BONDAGE_MANUAL_CLASS_FUNCTIONS_END(Qt, QTextEdit)
BONDAGE_MANUAL_CLASS(Qt, BONDAGE_NULL_NAMESPACE, QTextEdit, QAbstractScrollArea)

BONDAGE_MANUAL_CLASS_FUNCTIONS_BEGIN(Qt, QToolBar)
BONDAGE_MANUAL_CLASS_FUNCTIONS_END(Qt, QToolBar)
BONDAGE_MANUAL_CLASS(Qt, BONDAGE_NULL_NAMESPACE, QToolBar, QWidget)

BONDAGE_MANUAL_CLASS_FUNCTIONS_BEGIN(Qt, QMenu)
BONDAGE_MANUAL_CLASS_FUNCTIONS_END(Qt, QMenu)
BONDAGE_MANUAL_CLASS(Qt, BONDAGE_NULL_NAMESPACE, QMenu, QWidget)


void QApplication_exec(QApplication *a)
{
  a->exec();
}

BONDAGE_MANUAL_CLASS_FUNCTIONS_BEGIN(Qt, QApplication)
  BONDAGE_MANUAL_CLASS_INJECTED_METHOD(QApplication_exec, execute)
BONDAGE_MANUAL_CLASS_FUNCTIONS_END(Qt, QApplication)
BONDAGE_MANUAL_CLASS(Qt, BONDAGE_NULL_NAMESPACE, QApplication, BONDAGE_NULL_PARENT)
