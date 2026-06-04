# Engine/RHI

`RHI` 是 Render Hardware Interface，渲染硬件接口。

## 这个文件夹干什么？

定义 Renderer 使用的抽象渲染接口，让上层不用直接依赖 OpenGL。

## 这里应该放什么代码？

- `RenderDevice`
- `Buffer`
- `Texture`
- `Shader`
- `Pipeline`
- `Framebuffer`
- `Sampler`
- `CommandList`
- RHI 描述结构和枚举

## 它依赖谁？

可以依赖 Core 和 Math。不依赖 Scene，不包含 OpenGL 头文件。

## 完成到什么程度算合格？

- Renderer 可以只通过 RHI 创建资源和提交命令。
- OpenGL 后端可以完整实现 v0.1 接口。
- RHI 头文件没有图形 API 实现细节。

## 常见风险

- 抽象过度，第一版无法实现。
- 抽象不足，OpenGL 细节泄漏到 Renderer。
- RHI 认识 Entity 或 Component。

## 文件与函数职责表

### 文件：`Engine/RHI/RenderDevice.h`

职责：声明所有 GPU 资源和命令对象的创建入口。

- `RenderDevice::CreateBuffer(const BufferDesc&, const void* initialData)`
  - 作用：创建 vertex/index/uniform 等 GPU buffer。
  - 输入：buffer 描述和可选初始数据。
  - 输出：Buffer 句柄或对象。
  - 调用时机：Mesh 上传、uniform buffer 创建。
  - 依赖：RHI 后端实现。
  - 验收标准：Renderer 可以通过它创建 vertex/index buffer。
  - 常见风险：usage 标记不清导致后端无法正确创建。

- `RenderDevice::CreateTexture(const TextureDesc&, const void* initialData)`
  - 作用：创建 2D texture、render target 或 depth texture。
  - 输入：texture 描述和可选像素数据。
  - 输出：Texture 句柄或对象。
  - 调用时机：加载贴图、创建 GBuffer/HDR target。
  - 依赖：像素格式约定。
  - 验收标准：贴图和 framebuffer attachment 都能通过此接口创建。
  - 常见风险：颜色空间和格式混用。

- `RenderDevice::CreateShader(const ShaderDesc&)`
  - 作用：创建 shader program。
  - 输入：shader 文件路径或源码描述。
  - 输出：Shader 句柄或对象。
  - 调用时机：AssetManager 加载 Shader。
  - 依赖：ShaderLoader 和后端编译逻辑。
  - 验收标准：编译失败能返回无效对象并输出日志。
  - 常见风险：错误日志不包含文件名。

- `RenderDevice::CreatePipeline(const PipelineDesc&)`
  - 作用：创建渲染管线状态。
  - 输入：shader、顶点布局、深度、混合、剔除等状态。
  - 输出：Pipeline 对象。
  - 调用时机：材质或渲染 Pass 初始化。
  - 依赖：Shader、RHI 状态枚举。
  - 验收标准：不同 Pass 可以使用不同 Pipeline。
  - 常见风险：状态散落到 CommandList 中临时设置。

- `RenderDevice::CreateFramebuffer(const FramebufferDesc&)`
  - 作用：创建 render target 集合。
  - 输入：color/depth attachment 描述。
  - 输出：Framebuffer 对象。
  - 调用时机：GBuffer、HDR、Shadow map 创建。
  - 依赖：Texture。
  - 验收标准：Framebuffer 完整性可检查。
  - 常见风险：attachment 尺寸或格式不匹配。

- `RenderDevice::CreateCommandList()`
  - 作用：创建绘制命令提交对象。
  - 输入：无，后续可加入 queue type。
  - 输出：CommandList。
  - 调用时机：Renderer 每帧或初始化时。
  - 依赖：后端设备。
  - 验收标准：CommandList 能执行 draw indexed。
  - 常见风险：命令对象生命周期和 frame 生命周期混乱。

### 文件：`Engine/RHI/Buffer.h`

职责：声明 GPU buffer 抽象。

- `Buffer::Upload(const void* data, size_t size, size_t offset)`
  - 作用：更新 buffer 内容。
  - 输入：源数据、大小、偏移。
  - 输出：成功/失败状态。
  - 调用时机：动态 mesh、uniform 更新。
  - 依赖：buffer usage。
  - 验收标准：上传后渲染数据变化。
  - 常见风险：上传大小超过 buffer 容量。

- `Buffer::GetSize()` / `Buffer::GetUsage()`
  - 作用：查询 buffer 信息。
  - 输入：无。
  - 输出：大小和用途。
  - 调用时机：调试、验证、资源统计。
  - 依赖：BufferDesc。
  - 验收标准：返回创建时的描述信息。
  - 常见风险：CPU 描述与 GPU 对象不同步。

### 文件：`Engine/RHI/Texture.h`

职责：声明 GPU texture 抽象。

- `Texture::GetWidth()` / `GetHeight()` / `GetFormat()`
  - 作用：查询 texture 尺寸和格式。
  - 输入：无。
  - 输出：宽、高、格式。
  - 调用时机：创建 framebuffer、调试资源、resize。
  - 依赖：TextureDesc。
  - 验收标准：返回值与创建描述一致。
  - 常见风险：窗口 resize 后旧尺寸未更新。

### 文件：`Engine/RHI/Shader.h`

职责：声明 shader 抽象。

- `Shader::IsValid()`
  - 作用：判断 shader 是否编译链接成功。
  - 输入：无。
  - 输出：`bool`。
  - 调用时机：创建 Pipeline 前。
  - 依赖：后端编译状态。
  - 验收标准：编译失败时返回 false。
  - 常见风险：失败 shader 仍被绑定。

- `Shader::GetName()`
  - 作用：获取 shader 名称。
  - 输入：无。
  - 输出：字符串。
  - 调用时机：日志、调试、Profiler。
  - 依赖：ShaderDesc。
  - 验收标准：日志能定位 shader。
  - 常见风险：只记录匿名 ID，调试困难。

### 文件：`Engine/RHI/Pipeline.h`

职责：描述渲染状态。

- `PipelineDesc::Validate()`
  - 作用：检查 shader、顶点布局、状态组合是否有效。
  - 输入：无。
  - 输出：`bool` 或错误信息。
  - 调用时机：创建 Pipeline 前。
  - 依赖：RHI 枚举和 Shader。
  - 验收标准：缺少 shader 时能报错。
  - 常见风险：无效状态直到绘制时才暴露。

### 文件：`Engine/RHI/Framebuffer.h`

职责：声明 framebuffer 抽象。

- `Framebuffer::GetColorAttachment(uint32_t index)`
  - 作用：获取 color attachment。
  - 输入：attachment 索引。
  - 输出：Texture 句柄或引用。
  - 调用时机：RenderGraph 连接 Pass 输入输出。
  - 依赖：FramebufferDesc。
  - 验收标准：GBuffer attachment 可被 Lighting Pass 读取。
  - 常见风险：索引越界。

- `Framebuffer::GetDepthAttachment()`
  - 作用：获取 depth attachment。
  - 输入：无。
  - 输出：Texture 句柄或引用。
  - 调用时机：深度测试、调试显示、后续 shadow。
  - 依赖：FramebufferDesc。
  - 验收标准：GBuffer depth 可访问。
  - 常见风险：没有 depth attachment 时未处理。

### 文件：`Engine/RHI/CommandList.h`

职责：声明绘制命令接口。

- `CommandList::Begin()` / `End()`
  - 作用：开始和结束命令记录。
  - 输入：无。
  - 输出：无。
  - 调用时机：每个 Pass 执行时。
  - 依赖：后端命令状态。
  - 验收标准：命令记录状态严格成对。
  - 常见风险：Begin/End 嵌套错误。

- `CommandList::SetPipeline(PipelineHandle)`
  - 作用：绑定渲染管线状态。
  - 输入：Pipeline。
  - 输出：无。
  - 调用时机：Draw 前。
  - 依赖：有效 Pipeline。
  - 验收标准：不同材质/Pass 状态生效。
  - 常见风险：未绑定 Pipeline 就绘制。

- `CommandList::SetVertexBuffer(BufferHandle)` / `SetIndexBuffer(BufferHandle)`
  - 作用：绑定 mesh buffer。
  - 输入：buffer。
  - 输出：无。
  - 调用时机：Draw 前。
  - 依赖：有效 Buffer。
  - 验收标准：mesh 顶点和索引正确读取。
  - 常见风险：buffer 类型绑定错误。

- `CommandList::DrawIndexed(uint32_t indexCount, uint32_t firstIndex, int32_t vertexOffset)`
  - 作用：提交 indexed draw。
  - 输入：索引数量、起始索引、顶点偏移。
  - 输出：无。
  - 调用时机：渲染 Mesh 或 SubMesh。
  - 依赖：Pipeline、vertex/index buffer 已绑定。
  - 验收标准：立方体能正确绘制。
  - 常见风险：indexCount 错误导致越界。
