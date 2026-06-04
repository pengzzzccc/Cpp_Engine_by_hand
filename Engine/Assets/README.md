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
