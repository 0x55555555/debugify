import "../../Eks/EksBuild" as Eks;

Eks.Library {

    files: [
        "EksBindings/**/*",
        "ManualBindings/**/*",
        "include/**/*",
        "src/**/*"
    ]

  cpp.frameworks: base.concat(["Ruby"])
  cpp.includePaths: [
    ".",
    "include",
    "EksBindings"
  ]

  Depends { name: "EksCore" }
  Depends { name: "LldbDriver" }
  Depends { name: "Bondage" }


  Export {
    Depends { name: "cpp" }
    Depends { name: "EksCore" }
    Depends { name: "Bondage" }
    cpp.includePaths: [ "include" ]
  }
}
