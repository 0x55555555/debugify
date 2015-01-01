import "../../Eks/EksBuild" as Eks;

Eks.Library {
  property path bondagePath: "bondage/"
  property path reflectPath: bondagePath + "runtime/Reflect/"
  property string systemRubyPath: "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.10.sdk/System/Library/Frameworks/Ruby.framework/Versions/2.0/Headers/ruby/"

  files: [
        "*.*",
        bondagePath + "generators/Ruby/runtime/**/*",
        bondagePath + "runtime/include/**/*",
        bondagePath + "runtime/src/**/*",
        reflectPath + "include/**/*",
    ]

  cpp.defines: base.concat(["REFLECT_DESCRIPTIVE_EXCEPTIONS=1", "REFLECT_ASSERT=xAssert", "BONDAGE_HELPER"])

  cpp.frameworks: base.concat(["Ruby"])
  cpp.includePaths: [
    ".",
    bondagePath + "runtime/include",
    bondagePath + "generators/Ruby/runtime",
    reflectPath + "include",
    systemRubyPath
  ]

  Depends { name: "EksCore" }

  Export {
    Depends { name: "cpp" }
    Depends { name: "EksCore" }
    cpp.includePaths: [
      ".",
      bondagePath + "runtime/include",
      bondagePath + "generators/Ruby/runtime",
      reflectPath + "include",
      systemRubyPath
    ]

    cpp.defines: base.concat(["REFLECT_DESCRIPTIVE_EXCEPTIONS=1", "REFLECT_ASSERT=xAssert", "BONDAGE_HELPER"])
  }
}
