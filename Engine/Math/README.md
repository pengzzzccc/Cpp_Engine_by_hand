# Engine/Math

`Math` 是数学辅助模块。

## 这个文件夹干什么？

提供引擎常用空间数学类型和辅助函数，并配合 GLM 使用。

## 这里应该放什么代码？

- `Transform`
- `AABB`
- `Frustum`
- `Ray`
- 矩阵辅助函数
- 坐标系约定说明

## 它依赖谁？

可以依赖 GLM 和 Core。不要依赖 Scene、Renderer、RHI。

## 完成到什么程度算合格？

- Transform 能生成 model matrix。
- AABB 和 Frustum 能支持基础裁剪。
- Ray 能支持后续拾取。
- 坐标系约定在文档和代码中一致。

## 常见风险

- 矩阵乘法顺序混乱。
- 左手/右手坐标系混用。
- Math 模块混入业务逻辑。
