import "Eks/EksBuild" as Eks;

Eks.SubModule {
  name: "Debugger"
  toRoot: "."

  references: [
    "Eks/EksAll.qbs",
    "plugins/LldbDriver/LldbDriver.qbs",
    "plugins/DebugifyBindings/DebugifyBindings.qbs",
    "plugins/UI/UI.qbs",
    "plugins/UIBindings/UIBindings.qbs",
  ]
}
