import "../../Eks/EksBuild" as Eks;

Eks.Library {
  property path bondagePath: "bondage/"
  property path reflectPath: bondagePath + "runtime/Reflect/"
  property string rubyPath: "/Users/jorj/.rvm/rubies/ruby-2.2.0/"
  property string rubySystemPath: rubyPath + "include/ruby-2.2.0/x86_64-darwin14/"

  files: [
        "*.*",
        bondagePath + "generators/Ruby/runtime/**/*",
        bondagePath + "runtime/include/**/*",
        bondagePath + "runtime/src/**/*",
        reflectPath + "include/**/*",
        "EksBindings/**/*"
    ]

  cpp.defines: base.concat(["REFLECT_DESCRIPTIVE_EXCEPTIONS=1", "REFLECT_ASSERT=xAssert", "BONDAGE_HELPER"])

  cpp.libraryPaths: base.concat([rubyPath + "lib/"])
  cpp.dynamicLibraries: base.concat(["ruby"])
  cpp.includePaths: [
    ".",
    bondagePath + "runtime/include",
    bondagePath + "generators/Ruby/runtime",
    reflectPath + "include",
    rubyPath + "include/ruby-2.2.0",
    rubySystemPath,
    "EksBindings"
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
      rubyPath + "include/ruby-2.2.0",
      rubySystemPath,
      "EksBindings"
    ]

    cpp.libraryPaths: base.concat([rubyPath + "lib/"])
    cpp.dynamicLibraries: base.concat(["ruby"])

    cpp.defines: base.concat(["REFLECT_DESCRIPTIVE_EXCEPTIONS=1", "REFLECT_ASSERT=xAssert", "BONDAGE_HELPER"])
  }
}
