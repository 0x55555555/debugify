import "../../Eks/EksBuild" as Eks;

Eks.Library {

    files: [
        "ManualBindings/**/*",
        "include/**/*",
        "src/**/*"
    ]

  cpp.includePaths: [
    ".",
    "include",
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
