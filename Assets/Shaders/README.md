# Assets/Shaders

这个目录存放 Shader 文件。

## 这里应该放什么内容？

- Vertex shader。
- Fragment shader。
- 后续可放 compute shader。
- 公共 include 片段。

## 命名建议

- `GBuffer.vert`
- `GBuffer.frag`
- `Lighting.frag`
- `ToneMapping.frag`
- `PBR.glsl`

## 完成到什么程度算合格？

- Shader 文件路径能被 AssetManager 找到。
- Shader 编译失败时日志能显示文件名。
- Shader 参数命名和 Material/RHI 约定一致。
