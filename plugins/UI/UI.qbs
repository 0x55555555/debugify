import "../../Eks/EksBuild" as Eks;

Eks.Library {
  files: [
        "include/**/*.h",
        "src/**/*.cpp",
        "forms/**/*.ui"
    ]

  Depends { name: "Qt.core" }
  Depends { name: "Qt.gui" }
  Depends { name: "Qt.widgets" }

  Depends { name: "LldbDriver" }

  Qt.core.config: base.concat("DEBUG")

  Export {
    Depends { name: "cpp" }
    Depends { name: "Qt.core" }
    Depends { name: "Qt.gui" }
    Depends { name: "Qt.widgets" }
    cpp.includePaths: [ "include" ]
  }
}
