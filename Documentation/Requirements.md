# 需求文档

本文定义 `Cpp_Engine_by_hand` 的 v0.1 功能需求。它不是评分表，而是实现时的目标清单：每个功能都说明目标、所属模块、应该放在哪里、依赖谁、做到什么程度算完成。

优先级说明：

- Must：v0.1 必须完成，否则引擎主线不成立。
- Should：v0.1 强烈建议完成，可以明显提升可靠性和可调试性。
- Could：后续扩展目标，不阻塞 v0.1。

## 1. WSL2 + CMake + Ninja 构建

优先级：Must

功能目标：

建立稳定的 Linux C++ 构建流程，让项目能在 WSL2 Ubuntu 24.04 中配置、编译、运行、测试。

所属模块：

- 根目录 `CMakeLists.txt`
- `Engine/CMakeLists.txt`
- `Sandbox/CMakeLists.txt`
- `Tests/CMakeLists.txt`

需要放的代码类型：

- CMake target 定义。
- include 路径配置。
- 第三方依赖链接配置。
- 编译选项和 C++ 标准配置。

依赖关系：

- 依赖 Ubuntu 工具链、CMake、Ninja、g++。
- 不依赖具体引擎模块。

v0.1 必须完成：

- `Engine` 构建为静态库或对象库。
- `Sandbox` 构建为可执行程序并链接 `Engine`.
- `Tests` 可以通过 `ctest` 运行。
- 生成 `compile_commands.json` 供编辑器使用。

验收标准：

```bash
cmake -S . -B build -G Ninja
cmake --build build
ctest --test-dir build
```

常见风险：

- CMake 只构建单个练习文件，导致后续模块无法接入。
- include 路径靠编辑器猜，而不是由 CMake target 提供。
- `build/` 被提交到 Git。

## 2. Core 生命周期

优先级：Must

功能目标：

建立引擎最基础的生命体：`Application`。所有模块的初始化、主循环、关闭顺序都由 Core 统一管理。

所属模块：

- `Engine/Core`

需要创建的文件夹：

- `Engine/Core`

需要放的代码类型：

- `Application`
- `EngineContext`
- `Layer`
- `LayerStack`
- `Time`
- `Log`
- `Assert`
- `Event`

依赖关系：

- Core 不依赖 Renderer、Scene、Assets、Platform。
- Core 可以被所有其他模块依赖。

v0.1 必须完成：

- `Application::Init`
- `Application::Run`
- `Application::Shutdown`
- 主循环 delta time 计算。
- 模块初始化顺序和关闭顺序可追踪。

后续扩展：

- Job system。
- Memory allocator。
- Service registry。
- Engine config。

验收标准：

- 空程序可以启动和关闭。
- 日志能显示 `Init -> Run -> Shutdown`。
- 关闭顺序与初始化顺序相反。
- 没有全局变量控制整个引擎生命周期。

常见风险：

- 构造函数里做太多重资源初始化。
- Shutdown 后仍访问已释放模块。
- Core 反向依赖高层模块。

## 3. SDL2 窗口和输入

优先级：Must

功能目标：

通过 SDL2 创建窗口、接收输入、创建 OpenGL Context，同时隐藏平台细节。

所属模块：

- `Engine/Platform`

需要创建的文件夹：

- `Engine/Platform`

需要放的代码类型：

- `Window`
- `SDLWindow`
- `Input`
- `InputState`
- `PlatformEvent`
- `FileDialog` 后续可选

依赖关系：

- 依赖 Core。
- SDL2 细节只允许出现在 Platform 和 RHI/OpenGL 初始化边界。

v0.1 必须完成：

- 创建 1280x720 窗口。
- 支持窗口关闭事件。
- 支持键盘当前按下、本帧按下、本帧释放。
- 支持鼠标位置、鼠标移动量、鼠标按钮。
- 支持创建 OpenGL Context。

后续扩展：

- 游戏手柄输入。
- 高 DPI 窗口。
- 多窗口。
- 文件拖拽。

验收标准：

- Sandbox 可以打开窗口。
- 点击关闭按钮后程序正常退出。
- Camera 可以读取 WASD 和鼠标输入。
- Renderer 不直接包含 SDL2 事件处理逻辑。

常见风险：

- 输入状态没有按帧更新。
- OpenGL Context 创建顺序错误。
- Platform 层泄漏到 Scene 或 Renderer 高层逻辑中。

## 4. RHI 抽象

优先级：Must

功能目标：

建立现代渲染硬件接口，让 Renderer 面向抽象资源和命令，而不是直接写 OpenGL。

所属模块：

- `Engine/RHI`

需要创建的文件夹：

- `Engine/RHI`

需要放的代码类型：

- `RenderDevice`
- `Buffer`
- `Texture`
- `Shader`
- `Pipeline`
- `Framebuffer`
- `RenderPass`
- `CommandList`
- `Sampler`
- `ResourceHandle`
- RHI 枚举和描述结构

依赖关系：

- 依赖 Core、Math。
- 不依赖 Scene。
- 不包含 OpenGL 头文件。

v0.1 必须完成：

- Buffer 创建和销毁。
- Texture 创建和销毁。
- Shader 创建和销毁。
- Pipeline 描述。
- Framebuffer 描述。
- CommandList 基础绘制接口。

后续扩展：

- 多后端。
- 多线程命令提交。
- GPU resource lifetime tracking。
- Descriptor set 风格资源绑定。

验收标准：

- Renderer 可以通过 RHI 创建资源。
- Renderer 不出现 OpenGL API 调用。
- OpenGL 后端可以实现所有 v0.1 RHI 接口。

常见风险：

- 抽象过度，导致第一版无法落地。
- 抽象不足，导致 OpenGL 细节泄漏到 Renderer。
- RHI 直接理解 Scene 或 Entity。

## 5. OpenGL 后端

优先级：Must

功能目标：

实现 RHI 的 OpenGL 版本，让现代架构先落地到可运行画面。

所属模块：

- `Engine/RHI/OpenGL`

需要创建的文件夹：

- `Engine/RHI/OpenGL`

需要放的代码类型：

- `OpenGLRenderDevice`
- `OpenGLBuffer`
- `OpenGLTexture`
- `OpenGLShader`
- `OpenGLPipeline`
- `OpenGLFramebuffer`
- `OpenGLCommandList`
- `OpenGLUtils`

依赖关系：

- 依赖 RHI、Platform、Core。
- 可以包含 glad/OpenGL 头文件。
- 不依赖 Scene。

v0.1 必须完成：

- glad 初始化。
- OpenGL debug callback。
- Shader 编译和链接错误日志。
- VAO/VBO/IBO 创建。
- 2D Texture 上传。
- Framebuffer 创建。
- Draw indexed。

后续扩展：

- Uniform buffer。
- Shader storage buffer。
- Texture array。
- Compute shader。
- GPU timer query。

验收标准：

- 可以清屏。
- 可以绘制三角形。
- 可以绘制 mesh。
- 可以渲染到 GBuffer。
- OpenGL 错误能被日志捕获。

常见风险：

- 在 Context 创建前加载 OpenGL 函数。
- GPU 对象析构时 Context 已经销毁。
- Shader 错误不打印，导致黑屏难查。

## 6. Shader / Buffer / Texture / Pipeline

优先级：Must

功能目标：

建立渲染资源的核心类型，使 Mesh、材质和 RenderGraph 能用统一接口提交绘制。

所属模块：

- `Engine/RHI`
- `Engine/RHI/OpenGL`
- `Engine/Renderer`
- `Engine/Assets`

需要放的代码类型：

- 资源描述结构。
- 资源句柄。
- GPU 对象封装。
- 加载和缓存逻辑。

依赖关系：

- Shader 和 Texture 的加载由 Assets 管理。
- GPU 创建由 RHI 后端执行。
- Renderer 使用资源句柄或引用，不负责磁盘加载。

v0.1 必须完成：

- Vertex buffer。
- Index buffer。
- Shader program。
- Texture2D。
- Pipeline state。
- Framebuffer attachment。

验收标准：

- 同一个 Shader 不重复编译。
- 同一个 Texture 不重复加载。
- Pipeline 能描述深度测试、混合、背面剔除。
- 资源释放可追踪。

常见风险：

- CPU 资源和 GPU 资源所有权混在一起。
- Texture 坐标上下颠倒。
- Pipeline 状态靠散落的 OpenGL 调用修改。

## 7. RenderGraph 骨架

优先级：Must

功能目标：

用 RenderGraph 描述一帧内的渲染 Pass 和资源关系，让现代管线具有可扩展结构。

所属模块：

- `Engine/Renderer`

需要创建的文件夹：

- `Engine/Renderer`

需要放的代码类型：

- `RenderGraph`
- `RenderPassNode`
- `RenderResource`
- `RenderGraphBuilder`
- `FrameContext`
- `FrameRenderer`

依赖关系：

- 依赖 RHI、Core、Math。
- 不依赖具体 OpenGL。
- 可以读取 Scene 提交的 render packet。

v0.1 必须完成：

- 添加 Pass。
- 声明 Pass 读写资源。
- 按顺序执行 Pass。
- 管理 frame-local render target。

后续扩展：

- 自动资源别名。
- 自动 barrier。
- Pass 裁剪。
- 可视化 RenderGraph。

验收标准：

- 一帧至少包含 GBuffer、Lighting、ToneMapping、Present。
- 每个 Pass 的输入输出能在日志中打印。
- Renderer 不再硬编码所有绘制步骤到一个函数里。

常见风险：

- 第一版就追求完整工业 RenderGraph，导致无法实现。
- Pass 与真实 GPU 资源生命周期脱节。
- 资源名和资源句柄混乱。

## 8. Deferred GBuffer

优先级：Must

功能目标：

建立 Deferred Rendering 的第一步，把几何信息先写入 GBuffer，再在 Lighting Pass 中统一计算光照。

所属模块：

- `Engine/Renderer`
- `Engine/RHI`
- `Engine/Assets`

需要放的代码类型：

- `GBufferPass`
- `GBufferLayout`
- GBuffer Shader。
- Material 参数映射。

依赖关系：

- 依赖 Mesh、Material、Camera。
- 依赖 RHI Framebuffer 和 Texture。

v0.1 必须完成：

- Albedo/Material target。
- Normal target。
- Depth target。
- Camera matrices。
- MeshRenderer 提交。

后续扩展：

- Motion vector。
- Metallic/Roughness/AO 分离。
- Object ID buffer。

验收标准：

- GBuffer attachment 能被调试显示。
- 多个 Mesh 能写入 GBuffer。
- 深度测试正确。

常见风险：

- GBuffer 格式选择过度复杂。
- 法线空间不统一。
- sRGB 和 linear color 混乱。

## 9. 基础 PBR 材质

优先级：Must

功能目标：

用简单但现代的材质模型表达表面属性，为后续真实感渲染打基础。

所属模块：

- `Engine/Renderer`
- `Engine/Assets`

需要放的代码类型：

- `Material`
- `MaterialInstance`
- `PBRMaterialData`
- PBR Shader。

依赖关系：

- 依赖 Texture、Shader、RHI。
- Scene 通过 MeshRenderer 引用材质。

v0.1 必须完成：

- BaseColor。
- Metallic。
- Roughness。
- Normal 可后置。
- Directional Light 基础光照。

后续扩展：

- IBL。
- BRDF LUT。
- Clear coat。
- Emissive。

验收标准：

- 同一个 Mesh 可以换不同材质。
- 金属度和粗糙度变化能影响光照。
- 材质参数能从 AssetManager 获取。

常见风险：

- PBR 公式和颜色空间没有统一。
- 材质资源和 Shader 参数绑定混乱。
- 材质直接依赖 OpenGL。

## 10. HDR + Tone Mapping

优先级：Must

功能目标：

让 Lighting Pass 输出 HDR 颜色，再通过 Tone Mapping 转换到屏幕可显示范围。

所属模块：

- `Engine/Renderer`

需要放的代码类型：

- `HDRRenderTarget`
- `ToneMappingPass`
- `PostProcessPass`
- fullscreen triangle/quad 绘制逻辑。

依赖关系：

- 依赖 RHI Texture、Framebuffer、Pipeline。
- 输入来自 Lighting Pass。

v0.1 必须完成：

- HDR color buffer。
- Reinhard 或 ACES 简化 Tone Mapping。
- Gamma correction。

后续扩展：

- Bloom。
- Exposure control。
- Color grading。

验收标准：

- 高强度光源不会直接白爆。
- 输出颜色看起来稳定。
- Tone Mapping Pass 可以开关。

常见风险：

- 忘记 gamma correction。
- HDR target 格式不正确。
- 后处理 Pass 与 Present 逻辑混在一起。

## 11. Camera

优先级：Must

功能目标：

提供观察 3D 场景的基础相机，支持 Sandbox 中自由移动。

所属模块：

- `Engine/Scene`
- `Engine/Renderer`
- `Engine/Math`

需要放的代码类型：

- `Camera`
- `CameraComponent`
- `EditorCamera` 或 `FlyCameraController`
- View/Projection 计算。

依赖关系：

- 依赖 Math。
- 输入来自 Platform。
- Renderer 使用相机矩阵，不控制相机运动。

v0.1 必须完成：

- Perspective projection。
- View matrix。
- WASD 移动。
- 鼠标转向。

后续扩展：

- Orthographic camera。
- Camera stack。
- Frustum culling。

验收标准：

- Sandbox 可以从不同角度观察场景。
- 相机移动与帧率无关。
- Renderer 能正确使用 ViewProjection。

常见风险：

- 欧拉角顺序混乱。
- 鼠标输入未按 delta 处理。
- Projection 矩阵近远平面设置不合理。

## 12. Mesh

优先级：Must

功能目标：

用统一数据结构表示几何体，让 Renderer 能绘制三角形、立方体和导入模型。

所属模块：

- `Engine/Renderer`
- `Engine/Assets`
- `Engine/RHI`

需要放的代码类型：

- `Mesh`
- `SubMesh`
- `Vertex`
- `MeshLoader`
- GPU buffer 创建逻辑。

依赖关系：

- CPU mesh 数据由 Assets 管理。
- GPU buffer 由 RHI 创建。
- Scene 的 MeshRenderer 引用 Mesh。

v0.1 必须完成：

- 手写 cube mesh。
- Vertex position/normal/uv。
- Index buffer。
- Mesh handle。

后续扩展：

- Assimp 模型导入。
- Tangent。
- LOD。
- Skeletal mesh。

验收标准：

- 可以绘制立方体。
- 可以绘制多个 Mesh 实例。
- Mesh 资源可以被缓存复用。

常见风险：

- 顶点布局与 Shader 不一致。
- Mesh 数据重复上传。
- SubMesh 与材质绑定关系不清。

## 13. Scene + Component

优先级：Must

功能目标：

建立组件式对象系统，用 Entity 承载 Transform、Camera、MeshRenderer、Light 等组件。

所属模块：

- `Engine/Scene`

需要放的代码类型：

- `Scene`
- `Entity`
- `Component`
- `TransformComponent`
- `CameraComponent`
- `MeshRendererComponent`
- `LightComponent`
- `NameComponent`
- `HierarchyComponent`

依赖关系：

- 依赖 Core、Math、Renderer、Assets。
- 不依赖 Platform。
- 不直接调用 RHI/OpenGL。

v0.1 必须完成：

- 创建 Entity。
- 删除 Entity。
- Transform 组件。
- Camera 组件。
- MeshRenderer 组件。
- Directional Light 组件。

后续扩展：

- Parent/child hierarchy。
- Scene serialization。
- Script component。
- Physics component。

验收标准：

- Sandbox 场景有多个 Entity。
- 每个 Entity 可以独立 Transform。
- Renderer 能从 Scene 收集可绘制对象。
- 删除 Entity 不留下悬空组件。

常见风险：

- 第一版 ECS 过度复杂。
- 组件生命周期不跟 Entity 同步。
- Scene 直接创建 GPU 资源。

## 14. AssetManager

优先级：Must

功能目标：

统一管理 Shader、Texture、Mesh、Material 的加载、缓存、引用和释放。

所属模块：

- `Engine/Assets`

需要放的代码类型：

- `AssetManager`
- `AssetHandle`
- `TextureLoader`
- `ShaderLoader`
- `MeshLoader`
- `MaterialLoader`
- `AssetRegistry`

依赖关系：

- 依赖 Core、RHI。
- 可被 Renderer 和 Scene 使用。
- 不依赖 Sandbox。

v0.1 必须完成：

- 通过路径加载资源。
- 重复加载命中缓存。
- 关闭时释放资源。
- 缺失资源有清晰错误日志。

后续扩展：

- UUID。
- 热重载。
- 异步加载。
- 资源数据库。

验收标准：

- 同一路径 Texture 只加载一次。
- Shader 编译失败能定位文件。
- AssetManager Shutdown 后无资源泄漏。

常见风险：

- 路径大小写和分隔符不统一。
- 资源所有权分散。
- GPU 资源和磁盘资源生命周期混淆。

## 15. Sandbox 示例场景

优先级：Must

功能目标：

用一个真实可运行场景验证引擎 API 是否好用。

所属模块：

- `Sandbox`

需要放的代码类型：

- `SandboxApp`
- `DemoScene`
- 相机控制。
- 示例材质和 Mesh 创建。

依赖关系：

- 依赖 Engine。
- 不被 Engine 依赖。

v0.1 必须完成：

- 打开窗口。
- 渲染一个包含多个物体的 3D 场景。
- 支持自由相机。
- 展示 PBR 材质差异。
- 展示 HDR/Tone Mapping。

后续扩展：

- Debug overlay。
- 多 demo scene。
- 性能测试场景。

验收标准：

- 运行 Sandbox 能看到画面。
- 相机可移动。
- 至少一个物体使用贴图。
- 关闭时不崩溃。

常见风险：

- 把底层渲染逻辑写进 Sandbox。
- Demo 只验证单个三角形，无法暴露架构问题。
- Sandbox 依赖临时绝对路径。

## 16. Debug 和工具

优先级：Should

功能目标：

让引擎可观察、可调试、可定位问题。

所属模块：

- `Engine/Debug`
- `Engine/Tools`

需要放的代码类型：

- `DebugDraw`
- `Profiler`
- `GpuMarker`
- `ImGuiLayer`
- `SceneHierarchyPanel`
- `InspectorPanel`
- `AssetBrowserPanel`

依赖关系：

- Debug 可以依赖 Renderer 和 Core。
- Tools 可以依赖 Scene、Assets、Renderer。
- Runtime 不应该强依赖 Tools。

v0.1 必须完成：

- 日志。
- 基础 Debug Draw 可后置到 Should。
- 基础 CPU frame timing 可后置到 Should。

后续扩展：

- ImGui Inspector。
- RenderGraph viewer。
- GPU profiler。
- Material editor。

验收标准：

- 错误能定位到模块。
- Debug Draw 不影响正常渲染管线。
- Tools 可以在不启用时完全不参与运行。

常见风险：

- 工具代码污染 Runtime。
- Debug 资源生命周期不清。
- Profiler 数据采样影响性能判断。
