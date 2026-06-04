# Engine/Tools

`Tools` 是后置编辑器和可视化工具模块。

## 这个文件夹干什么？

规划 ImGui 工具面板，例如 Scene Hierarchy、Inspector、Asset Browser 和 RenderGraph Viewer。

## 这里应该放什么代码？

- `ImGuiLayer`
- `SceneHierarchyPanel`
- `InspectorPanel`
- `AssetBrowserPanel`
- `RenderGraphViewer`

## 它依赖谁？

可以依赖 Scene、Renderer、Assets、Core。Runtime 不应该依赖 Tools。

## 完成到什么程度算合格？

- v0.1 可以只保留规划。
- 后续实现时工具可以开关。
- 工具关闭时不影响 Sandbox。
- 工具面板不拥有底层引擎资源。

## 常见风险

- 过早做编辑器导致 Runtime 没有完成。
- 工具和运行时逻辑耦合。
- Inspector 直接修改不该暴露的内部状态。
