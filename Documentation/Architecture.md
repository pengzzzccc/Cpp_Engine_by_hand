# 架构设计

本文描述 `Cpp_Engine_by_hand` 的整体架构。目标是参考成熟引擎常见思路，但不照搬复杂工业实现。

## 设计参考

Unity：

- 借鉴组件式对象模型。
- 借鉴 Scene、GameObject、Component 的易理解结构。
- 不照搬 Unity 的编辑器驱动工作流。

Unreal：

- 借鉴模块化、渲染管线分层、资源和运行时分离。
- 不照搬庞大的 UObject 和反射系统。

Godot：

- 借鉴清晰的节点/场景思路和工具友好性。
- 不采用纯场景树作为唯一对象模型。

bgfx：

- 借鉴渲染 API 抽象思想。
- 不追求第一版多后端完整覆盖。

Hazel：

- 借鉴教学型 C++ 引擎的分阶段实现方式。
- 不停留在简单 OpenGL demo，v0.1 会规划现代渲染管线。

## 架构目标

- 代码结构服务学习，不追求一开始极致性能。
- 模块依赖单向，不让底层反向认识高层。
- Renderer 面向 RHI，不直接依赖 OpenGL。
- Scene 面向 Renderer 提交数据，不创建 GPU 资源。
- AssetManager 统一资源加载和缓存。
- Sandbox 只验证引擎能力，不承载引擎内部实现。

## 总体分层

```text
Sandbox / Tools
    |
Scene
    |
Renderer
    |
RHI
    |
RHI/OpenGL
    |
Platform
    |
Core
```

横向模块：

```text
Math   -> 被 Scene / Renderer / RHI 使用
Assets -> 被 Scene / Renderer 使用，并通过 RHI 创建 GPU 资源
Debug  -> 辅助 Core / Renderer / Tools 观察状态
Tests  -> 验证所有模块
```

## 目录结构

```text
Engine/
  Core/
  Platform/
  Math/
  RHI/
    OpenGL/
  Renderer/
  Scene/
  Assets/
  Debug/
  Tools/
Sandbox/
Assets/
Tests/
third_party/
Documentation/
```

## Core

职责：

- 引擎生命周期。
- 主循环。
- 日志和断言。
- 时间管理。
- 事件分发。
- Layer 管理。

Core 不应该知道窗口、OpenGL、场景和资源。

## Platform

职责：

- SDL2 窗口。
- 输入状态。
- 平台事件。
- OpenGL Context 创建边界。
- 文件系统入口。

Platform 只解决平台差异，不处理游戏对象和渲染算法。

## Math

职责：

- Transform。
- AABB。
- Frustum。
- Ray。
- 矩阵和向量辅助。

Math 使用 GLM，但不要把复杂业务逻辑放在 Math。

## RHI

职责：

- 定义渲染资源和命令接口。
- 隔离 Renderer 和具体图形 API。
- 表达 Buffer、Texture、Shader、Pipeline、Framebuffer、CommandList。

RHI 不知道 Scene，不知道 MeshRendererComponent。

## RHI/OpenGL

职责：

- 用 OpenGL 实现 RHI。
- 管理 OpenGL 对象生命周期。
- 转换 RHI 枚举到 OpenGL 枚举。
- 处理 OpenGL debug output。

OpenGL 头文件只应该出现在这个模块和极少数平台初始化边界。

## Renderer

职责：

- 一帧渲染流程。
- RenderGraph。
- GBuffer。
- Lighting。
- Shadow。
- PostProcess。
- Debug Draw。

Renderer 不直接读写 SDL2，不直接写 OpenGL，不拥有 Scene。

## Scene

职责：

- Entity 创建和销毁。
- Component 存储。
- Transform 更新。
- Camera、Light、MeshRenderer 组件。
- 向 Renderer 提交 render packet。

Scene 不应该负责编译 Shader 或上传 Texture。

## Assets

职责：

- 资源路径管理。
- Shader、Texture、Mesh、Material 加载。
- 缓存和引用。
- 资源关闭时统一释放。

Engine 内的 `Engine/Assets` 是代码模块；根目录的 `Assets` 是实际资源文件夹。

## Debug

职责：

- Debug Draw。
- Profiler。
- GPU marker。
- 日志辅助。

Debug 可以帮助观察引擎，但不能成为核心流程的必需依赖。

## Tools

职责：

- ImGui Inspector。
- Scene Hierarchy。
- Asset Browser。
- RenderGraph Viewer。

Tools 属于后置模块，v0.1 可以只规划，不必完整实现。

## Sandbox

职责：

- 创建 demo scene。
- 验证窗口、输入、渲染、资源、场景。
- 展示引擎 API 是否好用。

Sandbox 不应该包含 OpenGL 细节或底层资源管理逻辑。

## 生命周期

启动：

```text
Application::Init
-> Log::Init
-> Platform::CreateWindow
-> RHI::CreateDevice
-> Renderer::Init
-> AssetManager::Init
-> Scene::Load
-> Sandbox::OnStart
```

每帧：

```text
Platform::PollEvents
-> Input::Update
-> Scene::Update
-> Renderer::BeginFrame
-> RenderGraph::Build
-> RenderGraph::Execute
-> Renderer::EndFrame
-> Window::SwapBuffers
```

关闭：

```text
Sandbox::OnStop
-> Scene::Unload
-> AssetManager::Shutdown
-> Renderer::Shutdown
-> RHI::DestroyDevice
-> Platform::DestroyWindow
-> Log::Shutdown
```

## 数据流

```text
Assets on disk
-> AssetManager
-> CPU resources
-> RHI GPU resources
-> Scene components reference handles
-> Renderer builds render packets
-> RenderGraph schedules passes
-> RHI/OpenGL executes commands
-> Window presents frame
```

## 模块边界原则

- 谁创建资源，谁负责释放。
- 上层描述意图，下层执行细节。
- 高层不要 include 低层实现头文件。
- 资源句柄优先于裸指针。
- 日志必须能定位模块。
- 每个模块都要有最小可运行验收。
