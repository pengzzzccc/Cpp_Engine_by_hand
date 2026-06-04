# Assets/Materials

这个目录存放材质描述资源。

## 这里应该放什么内容？

- 材质参数文件。
- 贴图引用。
- PBR 参数，例如 BaseColor、Metallic、Roughness。

## 命名建议

- `Default.material`
- `Gold.material`
- `Plastic.material`

## 完成到什么程度算合格？

- MaterialLoader 可以读取材质描述。
- 材质能引用 `Textures` 中的贴图。
- 材质参数和 Shader 参数一致。
