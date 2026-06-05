# Cpp_Engine_by_hand

从零手写一个 C++ 现代渲染引擎。

这个仓库不是要一开始就复制 Unity、Unreal 或 Godot，而是用可学习、可维护的方式，从最小 C++ 程序逐步建设一个现代渲染引擎。第一版目标是 `RHI + OpenGL`：上层按现代渲染接口设计，底层先用 OpenGL 实现，未来保留 Vulkan 或其他图形 API 的扩展空间。

开发环境默认使用 `WSL2 + Ubuntu 24.04`，构建系统使用 `CMake + Ninja`。

## 项目定位

本项目同时包含三类内容：

- 教程文档：告诉你从哪里开始、每一步创建什么目录、写什么类型的代码。
- 需求文档：告诉你 v0.1 要实现哪些功能，每个功能做到什么程度算合格。
- 架构文档：告诉你模块之间如何依赖，为什么这样拆目录，未来如何扩展。

具体 C++ 实现代码由你逐步完成。文档只规定目标、边界、模块职责和验收标准。

```bash
# to build
 rm -rf build && cmake -B build && cmake --build build
```

## Topics Covered

- C++17 工程组织
- WSL2 Ubuntu 开发环境
- CMake + Ninja 构建
- Core 生命周期
- SDL2 窗口和输入
- RHI 渲染硬件接口
- OpenGL 渲染后端
- RenderGraph / FrameGraph
- Deferred Rendering
- GBuffer
- PBR 材质
- HDR 和 Tone Mapping
- Scene 与 Component
- AssetManager
- Debug Draw 和 Profiler
- Sandbox 示例程序
- QA 与 smoke test

## 文档导航

建议按下面顺序阅读：

1. [需求文档](Documentation/Requirements.md)：先知道 v0.1 到底要做什么。
2. [架构设计](Documentation/Architecture.md)：理解模块分层、目录职责和依赖方向。
3. [教程路线](Documentation/Tutorial.md)：按阶段推进，不跳步。
4. [渲染管线](Documentation/RenderPipeline.md)：理解现代渲染管线如何落到 OpenGL。
5. [测试计划](Documentation/QA.md)：每个阶段如何验收。
6. [术语表](Documentation/Glossary.md)：遇到不懂的英文技术名词先查这里。

模块文档：

- [Engine](Engine/README.md)
- [Core](Engine/Core/README.md)
- [Platform](Engine/Platform/README.md)
- [Math](Engine/Math/README.md)
- [RHI](Engine/RHI/README.md)
- [RHI/OpenGL](Engine/RHI/OpenGL/README.md)
- [Renderer](Engine/Renderer/README.md)
- [Scene](Engine/Scene/README.md)
- [Engine Assets](Engine/Assets/README.md)
- [Debug](Engine/Debug/README.md)
- [Tools](Engine/Tools/README.md)
- [Sandbox](Sandbox/README.md)
- [Top-level Assets](Assets/README.md)
- [Tests](Tests/README.md)
- [third_party](third_party/README.md)

## v0.1 目标

v0.1 是“能跑起来、架构方向正确、渲染链路完整”的第一个版本。

Must:

- WSL2 Ubuntu 24.04 下可以配置、构建、运行。
- `Application` 拥有清楚的 `Init -> Run -> Shutdown` 生命周期。
- SDL2 能创建窗口、处理键盘鼠标输入、创建 OpenGL Context。
- RHI 抽象存在，上层 Renderer 不直接依赖 OpenGL。
- OpenGL 后端能实现 Buffer、Texture、Shader、Pipeline、Framebuffer、CommandList 的基础能力。
- RenderGraph 骨架能表达 Pass 之间的资源读写关系。
- Deferred GBuffer 能输出位置/法线/材质等基础渲染数据。
- Lighting Pass 能用基础 PBR 模型渲染一个简单场景。
- HDR Render Target 和 Tone Mapping 能完成基础后处理。
- Scene 能创建 Entity，并挂载 Transform、Camera、MeshRenderer、Light。
- AssetManager 能加载并缓存 Shader、Texture、Mesh、Material。
- Sandbox 能展示一个简单 3D 场景。

Should:

- 支持 Directional Light shadow map。
- 支持 Debug Draw。
- 支持基础 Profiler。
- 支持 Shader reload。
- 支持 Frustum Culling。
- 支持 Renderer smoke test。

Could:

- 支持 ImGui Inspector。
- 支持 Scene Hierarchy。
- 支持 Asset Browser。
- 支持场景序列化。
- 支持 Vulkan 后端预留。

## 推荐目录

```text
Cpp_Engine_by_hand/
  README.md
  CMakeLists.txt
  Documentation/
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
    Shaders/
    Textures/
    Meshes/
    Materials/
    Scenes/
  Tests/
  third_party/
```

## 依赖方向

依赖必须保持单向：

```text
Sandbox -> Engine
Tools -> Scene / Renderer / Assets / Core
Scene -> Renderer / Assets / Math / Core
Renderer -> RHI / Assets / Math / Core
RHI/OpenGL -> RHI / Platform / Core
Platform -> Core
Assets -> RHI / Core
Debug -> Renderer / Core
Core -> no engine module dependency
```

重要原则：

- `Core` 不依赖任何高层模块。
- `Renderer` 不直接调用 SDL2，也不直接依赖 OpenGL。
- `RHI` 只描述渲染接口，不描述场景。
- `RHI/OpenGL` 才包含 OpenGL 细节。
- `Scene` 不负责创建 GPU 资源，只提交可渲染数据。
- `Sandbox` 是使用者，不是引擎内部模块。

## 构建命令

```bash
cmake -S . -B build -G Ninja
cmake --build build
```

运行示例程序：

```bash
./build/Sandbox/Sandbox
```

运行测试：

```bash
ctest --test-dir build
```

## 当前状态

当前仓库还处在早期学习和结构准备阶段。已经存在的 `Application` 代码可以作为 C++ 类、头文件、源文件和编译练习；后续实现时应逐步按照本文档体系重构为正式引擎模块。

下一步建议阅读 [教程路线](Documentation/Tutorial.md)，从工程结构和 Core 生命周期开始。
