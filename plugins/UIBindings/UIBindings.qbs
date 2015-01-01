import "../../Eks/EksBuild" as Eks;

Eks.Library {
  files: [
    "include/**/*",
    "src/**/*"
  ]

  cpp.frameworks: base.concat(["Ruby"])
  cpp.includePaths: [
    "include"
  ]

  Depends { name: "EksCore" }
  Depends { name: "UI" }
  Depends { name: "Bondage" }

  Export {
    Depends { name: "cpp" }
    Depends { name: "EksCore" }
    Depends { name: "Bondage" }
    cpp.includePaths: [ "include" ]
  }
}
