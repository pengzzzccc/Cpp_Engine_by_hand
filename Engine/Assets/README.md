# Engine/Assets

`Engine/Assets` 是资源管理代码模块。

## 这个文件夹干什么？

负责加载、缓存和释放 Shader、Texture、Mesh、Material 等资源。

## 这里应该放什么代码？

- `AssetManager`
- `AssetHandle`
- `AssetRegistry`
- `ShaderLoader`
- `TextureLoader`
- `MeshLoader`
- `MaterialLoader`

## 它依赖谁？

依赖 Core 和 RHI。可以被 Scene 和 Renderer 使用。不依赖 Sandbox。

## 完成到什么程度算合格？

- 同一路径资源不会重复加载。
- 缺失资源有清晰错误日志。
- Shutdown 时统一释放资源。
- CPU 资源和 GPU 资源生命周期清楚。

## 常见风险

- 路径处理不统一。
- 资源所有权分散到各个模块。
- 把资源文件夹 `Assets/` 和代码模块 `Engine/Assets` 混淆。

## 文件与函数职责表

### 文件：`Engine/Assets/AssetManager.h` / `AssetManager.cpp`

职责：统一加载、缓存、查询和释放资源。

- `AssetManager::Init(RenderDevice*)`
  - 作用：初始化资源系统，并保存创建 GPU 资源所需的 RHI device。
  - 输入：RenderDevice。
  - 输出：成功/失败状态。
  - 调用时机：Renderer/RHI 初始化后。
  - 依赖：Core、RHI。
  - 验收标准：后续能加载 shader、texture、mesh、material。
  - 常见风险：AssetManager 比 RHI 更早初始化。

- `AssetManager::Shutdown()`
  - 作用：释放所有缓存资源。
  - 输入：无。
  - 输出：无。
  - 调用时机：Scene 卸载后、RHI 销毁前。
  - 依赖：资源缓存和 RHI。
  - 验收标准：重复 Shutdown 安全。
  - 常见风险：GPU 资源释放时 device 已无效。

- `AssetManager::LoadTexture(path)`
  - 作用：加载图片并创建 RHI texture。
  - 输入：相对资源路径。
  - 输出：TextureHandle。
  - 调用时机：材质加载、Sandbox 创建资源。
  - 依赖：TextureLoader、RHI。
  - 验收标准：同一路径重复加载返回同一 handle。
  - 常见风险：颜色贴图和数据贴图格式混用。

- `AssetManager::LoadShader(vertexPath, fragmentPath)`
  - 作用：加载 shader 文件并创建 RHI shader。
  - 输入：shader 路径。
  - 输出：ShaderHandle。
  - 调用时机：材质或 pipeline 创建。
  - 依赖：ShaderLoader、RHI。
  - 验收标准：编译失败日志包含文件路径。
  - 常见风险：热重载时旧 shader 仍被 pipeline 引用。

- `AssetManager::LoadMesh(path)` / `LoadMaterial(path)`
  - 作用：加载 mesh 或 material。
  - 输入：资源路径。
  - 输出：MeshHandle 或 MaterialHandle。
  - 调用时机：场景加载、Sandbox 初始化。
  - 依赖：对应 Loader。
  - 验收标准：资源缺失时返回无效 handle 并记录错误。
  - 常见风险：material 引用 texture 时递归加载失败。

- `AssetManager::UnloadUnused()`
  - 作用：释放未被引用的资源。
  - 输入：无。
  - 输出：释放数量或无。
  - 调用时机：场景切换、内存整理。
  - 依赖：引用计数或资源状态。
  - 验收标准：仍在 Scene 使用的资源不会被释放。
  - 常见风险：引用跟踪不准导致正在使用资源被销毁。

### 文件：`Engine/Assets/AssetHandle.h`

- `AssetHandle::IsValid()`
  - 作用：判断 handle 是否指向有效资源。
  - 输入：无。
  - 输出：`bool`。
  - 调用时机：使用资源前。
  - 依赖：handle id。
  - 验收标准：无效资源不会进入 Renderer。
  - 常见风险：默认构造 handle 被当成有效资源。

- `AssetHandle::GetId()` / 比较函数
  - 作用：获取 ID 并支持 map/set 查找。
  - 输入：无或另一个 handle。
  - 输出：ID 或比较结果。
  - 调用时机：缓存、组件引用、调试。
  - 依赖：稳定 ID 生成。
  - 验收标准：同一资源 handle 可比较。
  - 常见风险：ID 重用导致旧引用误命中。

### Loader 文件

- `TextureLoader::Load(path, RenderDevice&)`
  - 作用：读取图片文件并创建 texture。
  - 输入：路径和 RenderDevice。
  - 输出：Texture 资源。
  - 调用时机：AssetManager LoadTexture。
  - 依赖：stb_image、RHI。
  - 验收标准：PNG/JPG 可加载。
  - 常见风险：图片上下翻转和通道数处理不一致。

- `ShaderLoader::Load(vertexPath, fragmentPath, RenderDevice&)`
  - 作用：读取 shader 源码并创建 RHI shader。
  - 输入：shader 路径和 RenderDevice。
  - 输出：Shader 资源。
  - 调用时机：AssetManager LoadShader。
  - 依赖：文件系统、RHI。
  - 验收标准：错误日志清楚。
  - 常见风险：include 片段路径无法解析。

- `MeshLoader::Load(path, RenderDevice&)`
  - 作用：读取 mesh 数据并上传 GPU buffer。
  - 输入：路径和 RenderDevice。
  - 输出：Mesh 资源。
  - 调用时机：AssetManager LoadMesh。
  - 依赖：手写 mesh 或 Assimp。
  - 验收标准：cube/obj 可加载。
  - 常见风险：顶点布局和 shader 不一致。

- `MaterialLoader::Load(path, AssetManager&)`
  - 作用：读取材质描述并加载依赖资源。
  - 输入：材质路径和 AssetManager。
  - 输出：Material 资源。
  - 调用时机：场景加载。
  - 依赖：Texture/Shader 加载。
  - 验收标准：PBR 参数和贴图能被解析。
  - 常见风险：材质格式没有版本字段。
