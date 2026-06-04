# Engine/Renderer

`Renderer` 是渲染系统的高层模块。

## 这个文件夹干什么？

负责一帧渲染流程、RenderGraph、Deferred GBuffer、Lighting、PostProcess 和 Debug Draw。

## 这里应该放什么代码？

- `Renderer`
- `FrameRenderer`
- `RenderGraph`
- `RenderPass`
- `RenderResource`
- `Mesh`
- `Material`
- `Camera`
- `GBufferPass`
- `LightingPass`
- `ToneMappingPass`
- `DebugDraw`

## 它依赖谁？

依赖 RHI、Assets、Math、Core。不要直接依赖 OpenGL 或 SDL2。

## 完成到什么程度算合格？

- 能通过 RHI 提交绘制。
- 能执行 GBuffer、Lighting、ToneMapping、Present。
- 能渲染一个有材质的 3D 场景。
- Pass 顺序和资源关系可追踪。

## 常见风险

- Renderer 直接写 OpenGL。
- RenderGraph 过度复杂。
- Scene 和 Renderer 互相持有强依赖。

## 文件与函数职责表

### 文件：`Engine/Renderer/Renderer.h` / `Renderer.cpp`

职责：渲染系统总入口。

- `Renderer::Init(RenderDevice*)`
  - 作用：初始化全局渲染资源和默认 pipeline。
  - 输入：RHI RenderDevice。
  - 输出：成功/失败状态。
  - 调用时机：RHI device 创建后。
  - 依赖：RHI、Assets。
  - 验收标准：Renderer 可以开始帧渲染。
  - 常见风险：Renderer 自己创建 OpenGL 后端。

- `Renderer::BeginFrame(const FrameContext&)`
  - 作用：开始一帧，重置临时状态。
  - 输入：frame context。
  - 输出：无。
  - 调用时机：每帧 Scene 更新后。
  - 依赖：FrameRenderer、RenderGraph。
  - 验收标准：每帧资源状态清空。
  - 常见风险：上一帧 render packet 残留。

- `Renderer::Submit(const RenderPacket&)`
  - 作用：接收 Scene 生成的绘制数据。
  - 输入：render packet。
  - 输出：无。
  - 调用时机：Scene `CollectRenderPackets` 后。
  - 依赖：Mesh、Material、Camera。
  - 验收标准：多个物体能被提交。
  - 常见风险：Submit 中直接绘制，绕过 RenderGraph。

- `Renderer::EndFrame()`
  - 作用：构建并执行 RenderGraph，完成一帧。
  - 输入：无。
  - 输出：无。
  - 调用时机：所有 Submit 完成后。
  - 依赖：FrameRenderer。
  - 验收标准：GBuffer、Lighting、ToneMapping、Present 执行。
  - 常见风险：EndFrame 和 Present 逻辑混乱。

- `Renderer::Shutdown()`
  - 作用：释放 Renderer 持有的持久资源。
  - 输入：无。
  - 输出：无。
  - 调用时机：AssetManager 前后顺序需要明确。
  - 依赖：RHI device 仍有效。
  - 验收标准：关闭无崩溃、无重复释放。
  - 常见风险：RHI device 已销毁后释放资源。

### 文件：`FrameRenderer.h` / `FrameRenderer.cpp`

- `FrameRenderer::BuildFrame(SceneRenderData)`
  - 作用：根据本帧场景数据构建 RenderGraph。
  - 输入：相机、灯光、render packets。
  - 输出：RenderGraph。
  - 调用时机：Renderer EndFrame 内。
  - 依赖：RenderGraph、Pass。
  - 验收标准：一帧 Pass 顺序可打印。
  - 常见风险：把 Scene 逻辑塞进 FrameRenderer。

- `FrameRenderer::ExecuteFrame(CommandList&)`
  - 作用：执行已经编译的 RenderGraph。
  - 输入：CommandList。
  - 输出：无。
  - 调用时机：BuildFrame 后。
  - 依赖：RHI。
  - 验收标准：所有 Pass 执行一次。
  - 常见风险：执行时再临时决定资源结构。

### 文件：`RenderGraph.h` / `RenderGraph.cpp`

- `RenderGraph::AddPass(name, setup, execute)`
  - 作用：添加渲染 Pass。
  - 输入：Pass 名称、setup 回调、execute 回调。
  - 输出：Pass handle。
  - 调用时机：FrameRenderer 构建帧时。
  - 依赖：RenderPass。
  - 验收标准：GBuffer/Lighting/ToneMapping 可注册。
  - 常见风险：Pass 名称重复难以调试。

- `RenderGraph::Read(resource)` / `Write(resource)`
  - 作用：声明 Pass 读取或写入的资源。
  - 输入：资源 handle 或描述。
  - 输出：资源访问记录。
  - 调用时机：Pass setup 阶段。
  - 依赖：RenderResource。
  - 验收标准：Pass 输入输出可追踪。
  - 常见风险：资源读写关系和实际执行不一致。

- `RenderGraph::Compile()`
  - 作用：验证 Pass 和资源关系，生成执行计划。
  - 输入：无。
  - 输出：成功/失败状态。
  - 调用时机：Execute 前。
  - 依赖：Pass 列表、资源表。
  - 验收标准：缺失资源能报错。
  - 常见风险：第一版做太复杂阻塞实现。

- `RenderGraph::Execute(CommandList&)`
  - 作用：按计划执行 Pass。
  - 输入：CommandList。
  - 输出：无。
  - 调用时机：每帧。
  - 依赖：RHI、RenderPass。
  - 验收标准：Pass 顺序与日志一致。
  - 常见风险：Pass 执行时修改图结构。

### 文件：`RenderPass.h`

- `RenderPass::Setup(RenderGraphBuilder&)`
  - 作用：声明资源读写和 pipeline 需求。
  - 输入：builder。
  - 输出：无。
  - 调用时机：RenderGraph 构建时。
  - 依赖：RenderGraph。
  - 验收标准：Pass 的输入输出明确。
  - 常见风险：Setup 中做真实绘制。

- `RenderPass::Execute(CommandList&, FrameContext&)`
  - 作用：执行真实绘制命令。
  - 输入：CommandList、FrameContext。
  - 输出：无。
  - 调用时机：RenderGraph Execute。
  - 依赖：RHI、Pass 数据。
  - 验收标准：命令只在 Execute 中提交。
  - 常见风险：Execute 中加载磁盘资源。

### 文件：`Mesh.h`

- `Mesh::CreateCube()`
  - 作用：生成测试立方体 CPU mesh。
  - 输入：可选尺寸。
  - 输出：Mesh。
  - 调用时机：Sandbox 或测试资源创建。
  - 依赖：Vertex 定义。
  - 验收标准：立方体顶点、法线、UV 正确。
  - 常见风险：面 winding 与剔除状态不一致。

- `Mesh::Upload(RenderDevice&)`
  - 作用：创建 GPU vertex/index buffer。
  - 输入：RenderDevice。
  - 输出：成功/失败状态。
  - 调用时机：Mesh 加载后。
  - 依赖：RHI Buffer。
  - 验收标准：mesh 可被 DrawIndexed 绘制。
  - 常见风险：重复上传。

- `Mesh::GetVertexBuffer()` / `GetIndexBuffer()`
  - 作用：返回绘制所需 buffer。
  - 输入：无。
  - 输出：buffer handle。
  - 调用时机：RenderPass 绘制 Mesh。
  - 依赖：Upload 已完成。
  - 验收标准：未上传时能安全报错。
  - 常见风险：返回悬空 handle。

### 文件：`Material.h`

- `Material::SetTexture(name, handle)`
  - 作用：设置材质贴图参数。
  - 输入：参数名和 texture handle。
  - 输出：无。
  - 调用时机：加载材质或运行时编辑。
  - 依赖：AssetManager。
  - 验收标准：Shader 能采样对应贴图。
  - 常见风险：参数名和 Shader 不一致。

- `Material::SetFloat(name, value)` / `SetVec3(name, value)`
  - 作用：设置 PBR 标量或向量参数。
  - 输入：参数名和值。
  - 输出：无。
  - 调用时机：材质加载和编辑。
  - 依赖：材质参数表。
  - 验收标准：Metallic/Roughness/BaseColor 可影响结果。
  - 常见风险：类型不匹配。

### 文件：`Camera.h`

- `Camera::GetViewMatrix()`
  - 作用：返回 view matrix。
  - 输入：无。
  - 输出：矩阵。
  - 调用时机：FrameContext 构建。
  - 依赖：Transform。
  - 验收标准：相机移动后画面正确变化。
  - 常见风险：相机朝向反了。

- `Camera::GetProjectionMatrix()`
  - 作用：返回 projection matrix。
  - 输入：无。
  - 输出：矩阵。
  - 调用时机：窗口尺寸或相机参数变化后。
  - 依赖：FOV、near、far、aspect。
  - 验收标准：透视比例正确。
  - 常见风险：near/far 设置不合理。

- `Camera::GetViewProjection()`
  - 作用：返回 Projection * View。
  - 输入：无。
  - 输出：矩阵。
  - 调用时机：渲染提交。
  - 依赖：View 和 Projection。
  - 验收标准：物体位置正确。
  - 常见风险：矩阵乘法顺序错误。

### 文件：`Passes/GBufferPass.*`、`LightingPass.*`、`ToneMappingPass.*`

- `Pass::Setup(RenderGraphBuilder&)`
  - 作用：声明各 Pass 输入输出资源。
  - 输入：builder。
  - 输出：无。
  - 调用时机：RenderGraph 构建。
  - 依赖：RenderGraph。
  - 验收标准：GBuffer 写入、Lighting 读取、ToneMapping 读取 HDR。
  - 常见风险：attachment 名称不一致。

- `Pass::Execute(CommandList&, FrameContext&)`
  - 作用：提交实际绘制命令。
  - 输入：CommandList 和 FrameContext。
  - 输出：无。
  - 调用时机：RenderGraph 执行。
  - 依赖：RHI、Mesh、Material、Shader。
  - 验收标准：最终窗口显示 tone mapped 画面。
  - 常见风险：Pass 内部直接访问 Scene。
