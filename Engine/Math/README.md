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

## 文件与函数职责表

### 文件：`Engine/Math/Transform.h`

职责：描述物体位置、旋转和缩放。

- `Transform::ToMatrix()`
  - 作用：生成 model matrix。
  - 输入：Transform 的 position、rotation、scale。
  - 输出：4x4 矩阵。
  - 调用时机：Scene 提交渲染数据时。
  - 依赖：GLM。
  - 验收标准：平移、旋转、缩放结果正确。
  - 常见风险：矩阵乘法顺序错误。

- `Transform::Forward()` / `Right()` / `Up()`
  - 作用：根据旋转计算方向向量。
  - 输入：Transform rotation。
  - 输出：单位方向向量。
  - 调用时机：相机移动、对象朝向、光照方向。
  - 依赖：坐标系约定。
  - 验收标准：默认朝向与 Renderer/Camera 约定一致。
  - 常见风险：左手/右手坐标系混用。

### 文件：`Engine/Math/AABB.h`

职责：表示轴对齐包围盒。

- `AABB::FromPoints(points)`
  - 作用：从点集生成包围盒。
  - 输入：顶点或点数组。
  - 输出：AABB。
  - 调用时机：Mesh 加载、碰撞或裁剪数据准备。
  - 依赖：向量类型。
  - 验收标准：min/max 覆盖所有点。
  - 常见风险：空点集没有处理。

- `AABB::Expand(pointOrBox)`
  - 作用：扩大当前包围盒。
  - 输入：点或另一个 AABB。
  - 输出：无或新 AABB。
  - 调用时机：合并子网格、更新层级包围盒。
  - 依赖：当前 min/max。
  - 验收标准：扩展后仍包含原始范围。
  - 常见风险：初始 min/max 未初始化。

- `AABB::Intersects(const AABB&)`
  - 作用：判断两个包围盒是否相交。
  - 输入：另一个 AABB。
  - 输出：`bool`。
  - 调用时机：粗略碰撞、空间查询。
  - 依赖：AABB min/max。
  - 验收标准：边界接触规则明确。
  - 常见风险：包含边界时返回规则不一致。

### 文件：`Engine/Math/Frustum.h`

职责：描述相机视锥。

- `Frustum::FromViewProjection(matrix)`
  - 作用：从 VP 矩阵提取视锥平面。
  - 输入：ViewProjection 矩阵。
  - 输出：Frustum。
  - 调用时机：相机更新后。
  - 依赖：矩阵约定。
  - 验收标准：六个平面方向正确。
  - 常见风险：矩阵行列主序理解错误。

- `Frustum::ContainsAABB(const AABB&)`
  - 作用：判断包围盒是否在视锥内。
  - 输入：AABB。
  - 输出：可见性结果。
  - 调用时机：Frustum culling。
  - 依赖：平面和 AABB。
  - 验收标准：相机外物体被裁掉。
  - 常见风险：误裁可见物体。

### 文件：`Engine/Math/Ray.h`

职责：表示射线，用于拾取和查询。

- `Ray::At(float t)`
  - 作用：返回射线上距离参数 t 对应的点。
  - 输入：标量 t。
  - 输出：三维点。
  - 调用时机：鼠标拾取、调试射线。
  - 依赖：origin 和 direction。
  - 验收标准：`At(0)` 返回 origin。
  - 常见风险：direction 没有归一化。

- `Ray::IntersectsAABB(const AABB&)`
  - 作用：判断射线是否与 AABB 相交。
  - 输入：AABB。
  - 输出：是否相交和可选距离。
  - 调用时机：对象选择、调试。
  - 依赖：AABB。
  - 验收标准：命中距离合理。
  - 常见风险：方向分量为 0 时除零。
