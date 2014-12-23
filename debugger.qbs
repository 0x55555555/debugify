import "Eks/EksBuild" as Eks;

Eks.SubModule {
  name: "Debugger"
  toRoot: "."

  references: [
    "Eks/EksAll.qbs",
    "plugins/lldb/lldb.qbs",
    "debugger/debugger.qbs",
    "test/test.qbs",
  ]
}
