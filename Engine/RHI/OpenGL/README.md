# Engine/RHI/OpenGL

`RHI/OpenGL` 是 RHI 的 OpenGL 实现。

## 这个文件夹干什么？

把 RHI 抽象转换成 OpenGL 调用，负责真实 GPU 资源创建、命令执行和错误检查。

## 这里应该放什么代码？

- `OpenGLRenderDevice`
- `OpenGLBuffer`
- `OpenGLTexture`
- `OpenGLShader`
- `OpenGLPipeline`
- `OpenGLFramebuffer`
- `OpenGLCommandList`
- `OpenGLUtils`

## 它依赖谁？

依赖 `Engine/RHI`、`Engine/Platform`、`Engine/Core` 和 glad/OpenGL。

## 完成到什么程度算合格？

- glad 初始化成功。
- 可以清屏和绘制 indexed mesh。
- Shader 编译错误会输出日志。
- GPU 资源在 Context 有效时释放。

## 常见风险

- 在 Context 创建前调用 OpenGL。
- 析构顺序导致 OpenGL object 泄漏或崩溃。
- OpenGL enum 转换散落在各处。

## 文件与函数职责表

### 文件：`OpenGLRenderDevice.h/.cpp`

职责：实现 RHI 的资源创建入口。

- `OpenGLRenderDevice::Init()`
  - 作用：初始化 glad、OpenGL debug callback 和默认状态。
  - 输入：有效 OpenGL Context。
  - 输出：成功/失败状态。
  - 调用时机：Platform 创建 Context 后。
  - 依赖：SDLWindow Context、glad。
  - 验收标准：能调用现代 OpenGL 函数。
  - 常见风险：Context 未创建就初始化。

- `OpenGLRenderDevice::CreateBuffer/CreateTexture/CreateShader/CreatePipeline/CreateFramebuffer/CreateCommandList`
  - 作用：创建对应 OpenGL 后端对象。
  - 输入：RHI 描述结构。
  - 输出：RHI 资源对象。
  - 调用时机：Renderer 或 AssetManager 创建资源时。
  - 依赖：OpenGL API、OpenGLUtils。
  - 验收标准：每个 RHI 创建函数都有对应 OpenGL 实现。
  - 常见风险：返回具体 OpenGL 类型导致上层依赖泄漏。

### 文件：`OpenGLBuffer.h/.cpp`

- `OpenGLBuffer::Create(const BufferDesc&, const void*)`
  - 作用：创建 GL buffer 并上传初始数据。
  - 输入：buffer 描述和可选数据。
  - 输出：成功/失败状态。
  - 调用时机：Mesh 上传或 uniform buffer 创建。
  - 依赖：`glCreateBuffers` / `glBufferData`。
  - 验收标准：vertex/index buffer 可用于绘制。
  - 常见风险：target/usage 转换错误。

- `OpenGLBuffer::Upload(const void*, size_t, size_t)`
  - 作用：更新已有 GL buffer。
  - 输入：数据、大小、偏移。
  - 输出：无或成功状态。
  - 调用时机：动态数据更新。
  - 依赖：有效 GL buffer id。
  - 验收标准：更新后的数据能反映到渲染。
  - 常见风险：越界上传。

- `OpenGLBuffer::Destroy()`
  - 作用：删除 GL buffer。
  - 输入：无。
  - 输出：无。
  - 调用时机：资源释放或 device shutdown 前。
  - 依赖：有效 Context。
  - 验收标准：不重复删除。
  - 常见风险：Context 销毁后才删除。

### 文件：`OpenGLTexture.h/.cpp`

- `OpenGLTexture::Create(const TextureDesc&, const void*)`
  - 作用：创建 GL texture，设置格式、尺寸、采样参数并上传数据。
  - 输入：texture 描述和可选像素数据。
  - 输出：成功/失败状态。
  - 调用时机：加载贴图或创建 render target。
  - 依赖：OpenGL format 转换。
  - 验收标准：纹理可被 shader 采样或 framebuffer 使用。
  - 常见风险：internal format 和 data format 不匹配。

- `OpenGLTexture::Bind(uint32_t slot)`
  - 作用：绑定 texture 到指定采样槽。
  - 输入：slot。
  - 输出：无。
  - 调用时机：Draw 前绑定材质纹理。
  - 依赖：有效 texture id。
  - 验收标准：shader 能采样正确贴图。
  - 常见风险：slot 与 shader uniform 不一致。

- `OpenGLTexture::Destroy()`
  - 作用：删除 GL texture。
  - 输入：无。
  - 输出：无。
  - 调用时机：AssetManager 或 Renderer 关闭时。
  - 依赖：有效 Context。
  - 验收标准：无泄漏、无重复释放。
  - 常见风险：framebuffer 还引用 texture 就删除。

### 文件：`OpenGLShader.h/.cpp`

- `OpenGLShader::Compile(stage, source)`
  - 作用：编译单个 shader stage。
  - 输入：stage 类型和源码。
  - 输出：stage id 或错误。
  - 调用时机：创建 Shader 时。
  - 依赖：OpenGL shader compiler。
  - 验收标准：编译错误打印源码文件和日志。
  - 常见风险：错误信息缺少行号上下文。

- `OpenGLShader::Link(stages)`
  - 作用：链接 shader program。
  - 输入：已编译 stage。
  - 输出：成功/失败状态。
  - 调用时机：所有 stage 编译成功后。
  - 依赖：OpenGL program。
  - 验收标准：program 可用于 Pipeline。
  - 常见风险：stage 删除时机错误。

- `OpenGLShader::Destroy()`
  - 作用：删除 program。
  - 输入：无。
  - 输出：无。
  - 调用时机：资源释放。
  - 依赖：有效 Context。
  - 验收标准：重复销毁安全。
  - 常见风险：Pipeline 仍引用已删除 shader。

### 文件：`OpenGLPipeline.h/.cpp`

- `OpenGLPipeline::Apply()`
  - 作用：应用 shader、深度、混合、剔除、顶点布局等状态。
  - 输入：无。
  - 输出：无。
  - 调用时机：CommandList `SetPipeline`。
  - 依赖：PipelineDesc、OpenGLUtils。
  - 验收标准：不同 Pass 状态正确切换。
  - 常见风险：状态泄漏到下一个 Pass。

### 文件：`OpenGLFramebuffer.h/.cpp`

- `OpenGLFramebuffer::Create(const FramebufferDesc&)`
  - 作用：创建 GL framebuffer 并绑定 attachments。
  - 输入：framebuffer 描述。
  - 输出：成功/失败状态。
  - 调用时机：GBuffer、HDR target 创建。
  - 依赖：OpenGLTexture。
  - 验收标准：`CheckComplete()` 成功。
  - 常见风险：attachment 尺寸不一致。

- `OpenGLFramebuffer::Bind()`
  - 作用：设置当前渲染目标。
  - 输入：无。
  - 输出：无。
  - 调用时机：RenderPass 执行开始。
  - 依赖：有效 framebuffer id。
  - 验收标准：渲染结果写入正确 attachment。
  - 常见风险：忘记恢复默认 framebuffer。

- `OpenGLFramebuffer::CheckComplete()`
  - 作用：检查 framebuffer 完整性。
  - 输入：无。
  - 输出：`bool`。
  - 调用时机：创建后。
  - 依赖：OpenGL 状态。
  - 验收标准：失败时输出详细原因。
  - 常见风险：只返回 false 不记录日志。

### 文件：`OpenGLCommandList.h/.cpp`

职责：实现 RHI 绘制命令。

- `Begin/End/SetPipeline/SetVertexBuffer/SetIndexBuffer/DrawIndexed`
  - 作用：把 RHI 命令翻译成 OpenGL 状态绑定和 draw call。
  - 输入：RHI 资源句柄和绘制参数。
  - 输出：无。
  - 调用时机：RenderGraph Pass 执行时。
  - 依赖：OpenGL backend resources。
  - 验收标准：Renderer smoke test 能清屏并绘制。
  - 常见风险：句柄类型转换不安全。

### 文件：`OpenGLUtils.h/.cpp`

- `ToGLFormat/ToGLUsage/ToGLCompare/ToGLBlend`
  - 作用：把 RHI 枚举转换成 OpenGL 枚举。
  - 输入：RHI 枚举。
  - 输出：OpenGL enum。
  - 调用时机：创建资源和应用状态时。
  - 依赖：RHIEnums。
  - 验收标准：所有 v0.1 枚举都有覆盖。
  - 常见风险：默认分支静默返回错误值。

- `CheckGLError()` / `SetDebugLabel()`
  - 作用：检查错误并给对象命名。
  - 输入：可选上下文字符串或对象 id。
  - 输出：错误状态。
  - 调用时机：调试构建、资源创建后。
  - 依赖：OpenGL debug extension。
  - 验收标准：日志能定位 OpenGL 错误位置。
  - 常见风险：每次调用都同步 GPU，影响性能。
