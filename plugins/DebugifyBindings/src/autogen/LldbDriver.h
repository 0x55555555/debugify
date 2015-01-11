// Autogenerated - do not change.

#pragma once
#include "TypeMember.h"
#include "Target.h"
#include "Process.h"
#include "FunctionMember.h"
#include "Frame.h"
#include "Thread.h"
#include "Debugger.h"
#include "CompileUnit.h"
#include "Type.h"
#include "Module.h"
#include "Breakpoint.h"
#include "../../BindingGenerator/EksBindings/EksBindings.h"
#include "../../DebugifyBindings/ManualBindings/ManualBindings.h"
#include "bondage/RuntimeHelpers.h"

namespace LldbDriver
{
EXPORT_DEBUGIFY const bondage::Library &bindings();
}

BONDAGE_EXPOSED_CLASS_COPYABLE(EXPORT_DEBUGIFY, ::LldbDriver::TypeMember)
BONDAGE_EXPOSED_CLASS_UNMANAGED(EXPORT_DEBUGIFY, ::LldbDriver::BreakpointNotifier)
BONDAGE_EXPOSED_CLASS_SHARED_POINTER(EXPORT_DEBUGIFY, ::LldbDriver::Process)
BONDAGE_EXPOSED_ENUM(EXPORT_DEBUGIFY, ::LldbDriver::Process::State)
BONDAGE_EXPOSED_CLASS_COPYABLE(EXPORT_DEBUGIFY, ::LldbDriver::FunctionMember)
BONDAGE_EXPOSED_CLASS_COPYABLE(EXPORT_DEBUGIFY, ::LldbDriver::Frame)
BONDAGE_EXPOSED_CLASS_SHARED_POINTER(EXPORT_DEBUGIFY, ::LldbDriver::Thread)
BONDAGE_EXPOSED_CLASS_SHARED_POINTER(EXPORT_DEBUGIFY, ::LldbDriver::Debugger)
BONDAGE_EXPOSED_CLASS_SHARED_POINTER(EXPORT_DEBUGIFY, ::LldbDriver::CompileUnit)
BONDAGE_EXPOSED_CLASS_COPYABLE(EXPORT_DEBUGIFY, ::LldbDriver::Type)
BONDAGE_EXPOSED_CLASS_SHARED_POINTER(EXPORT_DEBUGIFY, ::LldbDriver::Module)
BONDAGE_EXPOSED_CLASS_COPYABLE(EXPORT_DEBUGIFY, ::LldbDriver::Breakpoint)
BONDAGE_EXPOSED_CLASS_COPYABLE(EXPORT_DEBUGIFY, ::LldbDriver::BreakpointLocation)
BONDAGE_EXPOSED_CLASS_SHARED_POINTER(EXPORT_DEBUGIFY, ::LldbDriver::Target)

