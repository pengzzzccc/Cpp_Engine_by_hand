# 教程路线

本文按阶段说明如何从空项目推进到 v0.1 现代渲染引擎。每一步说明要创建什么文件夹、里面放什么类型代码、验收标准是什么。

## 阶段 0：环境和工程入口

目标：

确认 WSL2 Ubuntu 24.04、CMake、Ninja、g++ 可以工作。

需要创建：

- 根 `CMakeLists.txt`
- `Engine/CMakeLists.txt`
- `Sandbox/CMakeLists.txt`
- `Tests/CMakeLists.txt`

需要写的代码类型：

- CMake target。
- C++17 标准设置。
- `compile_commands.json` 导出。

验收：

```bash
cmake -S . -B build -G Ninja
cmake --build build
```

## 阶段 1：Core 生命周期

目标：

让引擎拥有清晰生命周期。

需要创建：

- `Engine/Core/Application.h`
- `Engine/Core/Application.cpp`
- `Engine/Core/Time.h`
- `Engine/Core/Log.h`
- `Engine/Core/Assert.h`
- `Engine/Core/Layer.h`
- `Engine/Core/LayerStack.h`

需要写的代码类型：

- Application 初始化、运行、关闭。
- 主循环。
- delta time。
- 简单日志。
- Layer 挂载和卸载。

验收：

- Sandbox 创建 Application。
- 程序能进入主循环并正常退出。
- 控制台打印生命周期顺序。

## 阶段 2：Platform 窗口和输入

目标：

通过 SDL2 打开窗口，并把输入封装到 Platform。

需要创建：

- `Engine/Platform/Window.h`
- `Engine/Platform/SDLWindow.h`
- `Engine/Platform/SDLWindow.cpp`
- `Engine/Platform/Input.h`
- `Engine/Platform/Input.cpp`
- `Engine/Platform/PlatformEvent.h`

需要写的代码类型：

- Window 抽象接口。
- SDLWindow 实现。
- SDL 事件轮询。
- 键盘、鼠标输入状态。
- OpenGL Context 创建入口。

验收：

- Sandbox 打开窗口。
- 点击关闭按钮退出。
- WASD 和鼠标输入可被读取。

## 阶段 3：RHI 接口

目标：

先定义现代渲染接口，再写具体 OpenGL。

需要创建：

- `Engine/RHI/RenderDevice.h`
- `Engine/RHI/Buffer.h`
- `Engine/RHI/Texture.h`
- `Engine/RHI/Shader.h`
- `Engine/RHI/Pipeline.h`
- `Engine/RHI/Framebuffer.h`
- `Engine/RHI/CommandList.h`
- `Engine/RHI/RHIEnums.h`

需要写的代码类型：

- RHI 描述结构。
- 资源接口。
- 命令接口。
- 渲染状态枚举。

验收：

- Renderer 可以 include RHI，不需要 include OpenGL。
- RHI 头文件不包含 glad/OpenGL。

## 阶段 4：OpenGL 后端

目标：

用 OpenGL 实现 RHI。

需要创建：

- `Engine/RHI/OpenGL/OpenGLRenderDevice.h`
- `Engine/RHI/OpenGL/OpenGLRenderDevice.cpp`
- `Engine/RHI/OpenGL/OpenGLBuffer.*`
- `Engine/RHI/OpenGL/OpenGLTexture.*`
- `Engine/RHI/OpenGL/OpenGLShader.*`
- `Engine/RHI/OpenGL/OpenGLPipeline.*`
- `Engine/RHI/OpenGL/OpenGLFramebuffer.*`
- `Engine/RHI/OpenGL/OpenGLCommandList.*`
- `Engine/RHI/OpenGL/OpenGLUtils.*`

需要写的代码类型：

- glad 初始化。
- OpenGL debug callback。
- Buffer/Texture/Shader/Pipeline/Framebuffer 创建。
- draw indexed。

验收：

- 可以清屏。
- 可以绘制一个三角形。
- Shader 编译失败能打印错误。

## 阶段 5：Renderer 基础

目标：

建立一帧渲染流程。

需要创建：

- `Engine/Renderer/Renderer.h`
- `Engine/Renderer/FrameRenderer.h`
- `Engine/Renderer/RenderPacket.h`
- `Engine/Renderer/Mesh.h`
- `Engine/Renderer/Material.h`
- `Engine/Renderer/Camera.h`

需要写的代码类型：

- BeginFrame / EndFrame。
- Mesh 提交。
- Camera 矩阵。
- Material 参数。

验收：

- 可以绘制一个旋转立方体。
- 可以移动相机观察。

## 阶段 6：RenderGraph

目标：

用 RenderGraph 管理一帧 Pass。

需要创建：

- `Engine/Renderer/RenderGraph.h`
- `Engine/Renderer/RenderGraph.cpp`
- `Engine/Renderer/RenderPass.h`
- `Engine/Renderer/RenderResource.h`
- `Engine/Renderer/FrameContext.h`

需要写的代码类型：

- Pass 注册。
- 资源声明。
- Pass 顺序执行。
- frame-local resource 管理。

验收：

- 一帧至少执行 GBuffer、Lighting、ToneMapping、Present。
- 日志能打印 Pass 顺序。

## 阶段 7：Deferred + GBuffer

目标：

把几何信息写入 GBuffer，为 PBR 光照准备数据。

需要创建：

- `Engine/Renderer/Passes/GBufferPass.*`
- `Engine/Renderer/Passes/LightingPass.*`
- `Assets/Shaders/GBuffer.vert`
- `Assets/Shaders/GBuffer.frag`
- `Assets/Shaders/Lighting.frag`

需要写的代码类型：

- GBuffer attachments。
- MeshRenderer 绘制。
- Lighting fullscreen pass。

验收：

- GBuffer 可以显示 albedo、normal、depth。
- Lighting Pass 输出基础光照结果。

## 阶段 8：PBR + HDR + Tone Mapping

目标：

让渲染管线具备现代材质和后处理基础。

需要创建：

- `Engine/Renderer/PBR.*`
- `Engine/Renderer/Passes/ToneMappingPass.*`
- `Assets/Shaders/PBR.glsl`
- `Assets/Shaders/ToneMapping.frag`

需要写的代码类型：

- BaseColor、Metallic、Roughness。
- Directional Light。
- HDR render target。
- Tone Mapping。
- Gamma correction。

验收：

- 材质参数会影响光照。
- 高亮不会直接溢出成纯白。
- 输出颜色稳定。

## 阶段 9：Scene + Component

目标：

把渲染 demo 升级为场景系统。

需要创建：

- `Engine/Scene/Scene.h`
- `Engine/Scene/Entity.h`
- `Engine/Scene/Components.h`
- `Engine/Scene/SceneRendererBridge.h`

需要写的代码类型：

- Entity ID。
- Component 存储。
- Transform、Camera、MeshRenderer、Light。
- Scene 提交 render packet。

验收：

- 多个 Entity 可以渲染。
- 每个 Entity 有独立 Transform。
- 相机和灯光来自 Scene。

## 阶段 10：AssetManager

目标：

统一资源加载和缓存。

需要创建：

- `Engine/Assets/AssetManager.h`
- `Engine/Assets/AssetHandle.h`
- `Engine/Assets/TextureLoader.*`
- `Engine/Assets/ShaderLoader.*`
- `Engine/Assets/MeshLoader.*`
- `Engine/Assets/MaterialLoader.*`

需要写的代码类型：

- 路径规范化。
- 资源缓存。
- 资源句柄。
- 缺失资源错误日志。

验收：

- 同一路径资源只加载一次。
- Shader、Texture、Mesh、Material 可以被 Scene 使用。

## 阶段 11：Debug 和测试

目标：

让引擎能被观察和验证。

需要创建：

- `Engine/Debug/DebugDraw.*`
- `Engine/Debug/Profiler.*`
- `Tests/CoreTests.cpp`
- `Tests/MathTests.cpp`
- `Tests/AssetTests.cpp`
- `Tests/RendererSmokeTests.cpp`

需要写的代码类型：

- CPU timer。
- Debug line/sphere。
- 生命周期测试。
- 渲染 smoke test。

验收：

- `ctest --test-dir build` 可运行。
- 渲染 smoke test 能创建窗口、清屏、关闭。

## 阶段 12：Tools 后置

目标：

规划编辑器工具，不阻塞 v0.1。

需要创建：

- `Engine/Tools/ImGuiLayer.*`
- `Engine/Tools/SceneHierarchyPanel.*`
- `Engine/Tools/InspectorPanel.*`
- `Engine/Tools/AssetBrowserPanel.*`

需要写的代码类型：

- ImGui 初始化。
- Scene 查看。
- Component 编辑。
- Asset 浏览。

验收：

- 工具可开关。
- 工具关闭时不影响 Runtime。

## 文件与函数阅读索引

实现每个阶段时，按这个顺序阅读模块 README 中的“文件与函数职责表”：

1. Core：先读 `Engine/Core/README.md`，建立 `Application`、`Layer`、`LayerStack`、`Time`、`Log`、`Assert` 的接口。
2. Platform：再读 `Engine/Platform/README.md`，实现 SDL2 窗口、输入和 OpenGL Context 边界。
3. Math：阅读 `Engine/Math/README.md`，补 Transform、AABB、Frustum、Ray。
4. RHI：阅读 `Engine/RHI/README.md`，先写抽象接口，不写 OpenGL。
5. RHI/OpenGL：阅读 `Engine/RHI/OpenGL/README.md`，再用 OpenGL 实现 RHI。
6. Renderer：阅读 `Engine/Renderer/README.md`，实现 Renderer、FrameRenderer、RenderGraph、Mesh、Material、Camera 和渲染 Pass。
7. Scene：阅读 `Engine/Scene/README.md`，实现 Entity 和 Component，并向 Renderer 提交 render packet。
8. Assets：阅读 `Engine/Assets/README.md`，实现 AssetManager 和各类 Loader。
9. Debug：阅读 `Engine/Debug/README.md`，实现 DebugDraw、Profiler、GpuMarker。
10. Tools：阅读 `Engine/Tools/README.md`，后置实现 ImGui 工具。
11. Sandbox：阅读 `Sandbox/README.md`，用 demo 场景验证引擎 API。
12. Tests：阅读 `Tests/README.md`，按阶段补测试。

推荐实现顺序：

```text
写接口函数
-> 写最小实现
-> 接入 CMake
-> 接入 Sandbox
-> 写测试或 smoke test
-> 回到文档确认验收标准
```

不要一开始追求完整实现。每个函数先做到“能表达职责、能被调用、能被验证”，再逐步补复杂逻辑。
