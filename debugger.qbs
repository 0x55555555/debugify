import "Eks/EksBuild" as Eks;

Eks.SubModule {
  name: "Debugger"
  toRoot: "."

  references: [
    "Eks/EksAll.qbs",
    "plugins/LldbDriver/LldbDriver.qbs",
    "plugins/Ruby/Ruby.qbs",
    "debugger/debugger.qbs",
    "test/test.qbs",
  ]
}
