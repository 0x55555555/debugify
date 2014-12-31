import "../../Eks/EksBuild" as Eks;

Eks.Library {
    files: [
        "EksBindings/**/*",
        "ManualBindings/**/*",
        "include/**/*",
        "src/**/*",
        "generator/bondage/generators/Ruby/runtime/**/*",
        "generator/bondage/runtime/include/**/*",
        "generator/bondage/runtime/src/**/*",
        "generator/bondage/runtime/Reflect/include/**/*",
    ]

  cpp.defines: base.concat(["REFLECT_DESCRIPTIVE_EXCEPTIONS=1"])

  cpp.frameworks: base.concat(["Ruby"])
  cpp.includePaths: [
    ".",
    "include",
    "EksBindings",
    "generator/bondage/runtime/include",
    "generator/bondage/runtime/Reflect/include",
    "generator/bondage/generators/Ruby/runtime",
    "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.10.sdk/System/Library/Frameworks/Ruby.framework/Versions/2.0/Headers/ruby/"
  ]

  Depends { name: "EksCore" }
  Depends { name: "LldbDriver" }


  Export {
    Depends { name: "cpp" }
    Depends { name: "EksCore" }
    cpp.includePaths: [ "include" ]
  }
}
