#pragma once
#include "XGlobal.h"
#include "Utilities/XMacroHelpers.h"
#include "Utilities/XPrivateImpl.h"

#ifdef LLDBDRIVER_BUILD
# define LLDB_DRIVER_EXPORT X_DECL_EXPORT
#else
# define LLDB_DRIVER_EXPORT X_DECL_IMPORT
#endif


#define SHARED_CLASS(cls) private: X_DISABLE_COPY(cls) public: typedef std::shared_ptr<cls> Pointer

#define PIMPL_CLASS(cls, size) private: class Impl; Eks::PrivateImplTyped<Impl, size> _impl
