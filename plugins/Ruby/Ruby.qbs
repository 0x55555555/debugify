import "../../Eks/EksBuild" as Eks;

Eks.Library {
    targetName: "Debugify"
    files: [
        "test.cpp",
        "test.h",
    ]

  cpp.frameworks: base.concat(["Ruby"])
  cpp.includePaths: [
    "include",
    "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.10.sdk/System/Library/Frameworks/Ruby.framework/Versions/2.0/Headers/ruby/"
  ]

  Depends { name: "EksCore" }


  Export {
    Depends { name: "cpp" }
    Depends { name: "EksCore" }
    cpp.includePaths: [ "include" ]
  }
}
