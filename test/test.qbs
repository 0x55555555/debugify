import "../Eks/EksBuild" as Eks;

Eks.Application {
  files: [ "*.h", "*.cpp", "*.ui" ]

  Depends { name: "Qt.core" }
}
