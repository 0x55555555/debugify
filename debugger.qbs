import "Eks/EksBuild" as Eks;

Eks.SubModule {
  name: "Debugger"
  toRoot: "."

  references: [
    "App/App.qbs",
    "Eks/EksAll.qbs",
    "plugins/BindingGenerator/Bondage.qbs",
    "plugins/LldbDriver/LldbDriver.qbs",
    "plugins/DebugifyBindings/DebugifyBindings.qbs",
    "plugins/UI/UI.qbs",
    "plugins/UIBindings/UIBindings.qbs",
    "testClient/test.qbs"
  ]
}
