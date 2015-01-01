ROOT="../../"
LLVM_BUILD_ROOT=$ROOT/llvm-build/Release+Asserts/
DEBUGGER_BUILD_ROOT=$ROOT/build-debugger-Desktop-Debug/qtc_Desktop-debug/

DYLD_LIBRARY_PATH=$LLVM_BUILD_ROOT/lib:$DEBUGGER_BUILD_ROOT/LldbDriver.qtc_Desktop:$DEBUGGER_BUILD_ROOT/UI.qtc_Desktop
