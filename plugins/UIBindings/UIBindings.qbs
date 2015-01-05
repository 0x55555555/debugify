import "../../Eks/EksBuild" as Eks;

Eks.Library {
  files: [
        "QtBindings/**/*",
        "include/**/*",
        "src/**/*",
    ]

  cpp.includePaths: [
    "include",
    "QtBindings"
  ]

  Depends { name: "EksCore" }
  Depends { name: "UI" }
  Depends { name: "Bondage" }
  Depends { name: "LldbDriver" }
  Depends { name: "DebugifyBindings" }

  Export {
    Depends { name: "cpp" }
    Depends { name: "EksCore" }
    Depends { name: "Bondage" }
    Depends { name: "LldbDriver" }
    cpp.includePaths: [ "include" ]
  }
}
