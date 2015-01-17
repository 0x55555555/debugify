ROOT=`pwd`"/../../"
LLVM_BUILD_ROOT=$ROOT/llvm-build/Debug+Asserts/
BUILD_ID=Desktop_Qt_5_4_0_clang_64bit
BUILD_VARIANT=qtc_$BUILD_ID
DEBUGGER_BUILD_ROOT=$ROOT/build-debugger-$BUILD_ID-Debug/$BUILD_VARIANT-debug/
PLUGINS_ROOT="../plugins/"

export LLDB_DEBUGSERVER_PATH=$LLVM_BUILD_ROOT/bin/debugserver
export DYLD_LIBRARY_PATH=$LLVM_BUILD_ROOT/lib/:$DEBUGGER_BUILD_ROOT/LldbDriver.qtc_Desktop:$DEBUGGER_BUILD_ROOT/UI.qtc_Desktop

export DEBUGIFY_DEBUG=1

ln -s -f $DEBUGGER_BUILD_ROOT/UIBindings.$BUILD_VARIANT/libUIBindings.dylib $PLUGINS_ROOT/UIBindings/ruby/UIBindings.bundle
ln -s -f $DEBUGGER_BUILD_ROOT/DebugifyBindings.$BUILD_VARIANT/libDebugifyBindings.dylib $PLUGINS_ROOT/DebugifyBindings/ruby/DebugifyBindings.bundle