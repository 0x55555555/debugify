import "../../Eks/EksBuild" as Eks;

Eks.Library {
  property path bondagePath: "../BindingGenerator/bondage/"
  property path reflectPath: bondagePath + "/runtime/Reflect/"

    files: [
        "ManualBindings/**/*",
        "include/**/*",
        "src/**/*",
        bondagePath + "generators/Ruby/runtime/**/*",
        bondagePath + "runtime/include/**/*",
        bondagePath + "runtime/src/**/*",
        reflectPath + "Reflect/include/**/*",
    ]

  cpp.defines: base.concat(["REFLECT_DESCRIPTIVE_EXCEPTIONS=1"])

  cpp.frameworks: base.concat(["Ruby"])
  cpp.includePaths: [
    ".",
    "include",
    bondagePath + "runtime/include",
    bondagePath + "generators/Ruby/runtime",
    reflectPath + "include",
    "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.10.sdk/System/Library/Frameworks/Ruby.framework/Versions/2.0/Headers/ruby/"
  ]

  Depends { name: "EksCore" }
  Depends { name: "UI" }


  Export {
    Depends { name: "cpp" }
    Depends { name: "EksCore" }
    cpp.includePaths: [ "include" ]
  }
}
