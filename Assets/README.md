# Assets

根目录 `Assets` 是运行时资源文件夹，不是 C++ 代码模块。

## 这个文件夹干什么？

保存 Shader、贴图、模型、材质和场景资源。

## 这里应该放什么代码或内容？

这里不放 C++ 引擎代码。这里放运行时资源文件，以及少量资源格式说明文档。

## 子目录

- `Shaders`：GLSL shader 文件。
- `Textures`：PNG、JPG、HDR 等贴图。
- `Meshes`：OBJ、glTF 或后续模型文件。
- `Materials`：材质描述文件。
- `Scenes`：场景描述文件。

## 它依赖谁？

资源文件不依赖代码，但会被 `Engine/Assets` 加载，被 Renderer 和 Scene 使用。

## 完成到什么程度算合格？

- 资源路径相对项目根目录稳定。
- Shader 命名清楚。
- Texture 和 Material 能对应。
- Sandbox 不使用绝对路径。

## 常见风险

- 资源路径大小写不一致。
- Shader 和材质参数命名不一致。
- 把生成的临时文件放进资源目录。
