import "../../Eks/EksBuild" as Eks;

Eks.Library {
  property path llvmPath: "../../../llvm/"
  property string llvmBuildType: "Debug+Asserts"
  property path llvmBuildPath: "../../../llvm-build/" + llvmBuildType + "/"
  property path lldbPath: llvmPath + "tools/lldb/"

  files: base.concat([ "internalInclude/**/*" ])

  cpp.includePaths: [ "include", "internalInclude", lldbPath + "include" ]

  cpp.libraryPaths: base.concat( [ llvmBuildPath + "lib/" ] )
  cpp.dynamicLibraries: [ "lldb" ]

  Depends { name: "EksCore" }

  Export {
    Depends { name: "cpp" }
    Depends { name: "EksCore" }
    cpp.includePaths: [ "include" ]
  }
}
