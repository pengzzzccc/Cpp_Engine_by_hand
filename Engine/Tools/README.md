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

## 文件与函数职责表

### 文件：`Engine/Tools/ImGuiLayer.h` / `ImGuiLayer.cpp`

- `ImGuiLayer::OnAttach()` / `OnDetach()`
  - 作用：初始化和关闭 ImGui 上下文与后端。
  - 输入：无。
  - 输出：无。
  - 调用时机：工具层加入或移除 LayerStack 时。
  - 依赖：Platform、Renderer。
  - 验收标准：工具层可开关。
  - 常见风险：ImGui 资源在 Renderer 后释放。

- `ImGuiLayer::Begin()` / `End()`
  - 作用：开始和结束一帧 UI 绘制。
  - 输入：无。
  - 输出：无。
  - 调用时机：每帧工具面板绘制前后。
  - 依赖：ImGui。
  - 验收标准：面板能稳定显示。
  - 常见风险：Begin/End 顺序错误。

### 文件：`SceneHierarchyPanel.h/.cpp`

- `SceneHierarchyPanel::SetScene(Scene*)`
  - 作用：指定当前显示的 Scene。
  - 输入：Scene 指针。
  - 输出：无。
  - 调用时机：加载或切换场景时。
  - 依赖：Scene。
  - 验收标准：面板显示当前 Scene entity。
  - 常见风险：Scene 已卸载但面板仍持有指针。

- `SceneHierarchyPanel::Draw()`
  - 作用：绘制 Entity 列表和选择状态。
  - 输入：无。
  - 输出：选中的 Entity 或内部状态。
  - 调用时机：每帧 ImGui 绘制。
  - 依赖：ImGui、Scene。
  - 验收标准：可以选择 Entity。
  - 常见风险：Draw 中修改 Scene 导致迭代失效。

### 文件：`InspectorPanel.h/.cpp`

- `InspectorPanel::DrawEntity(Entity)`
  - 作用：显示并编辑选中 Entity 的组件。
  - 输入：Entity。
  - 输出：无。
  - 调用时机：每帧工具绘制。
  - 依赖：Scene、组件系统、ImGui。
  - 验收标准：Transform、MeshRenderer、Light 可查看。
  - 常见风险：直接暴露内部资源指针。

### 文件：`AssetBrowserPanel.h/.cpp`

- `AssetBrowserPanel::Draw()`
  - 作用：显示资源目录和资源状态。
  - 输入：无。
  - 输出：用户选择的资源或内部状态。
  - 调用时机：每帧工具绘制。
  - 依赖：AssetManager、文件系统、ImGui。
  - 验收标准：可以浏览 `Assets` 下资源。
  - 常见风险：同步扫描大目录导致卡顿。
