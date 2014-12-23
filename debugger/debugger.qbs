import "../Eks/EksBuild" as Eks;

Eks.Application {
  files: [ "*.h", "*.cpp", "*.ui" ]

  Depends { name: "Qt.core" }
  Depends { name: "Qt.gui" }
  Depends { name: "Qt.widgets" }

  Depends { name: "LldbDriver" }
}
