# Tests

`Tests` 是测试目录。

## 这个文件夹干什么？

验证引擎核心逻辑、资源管理和渲染 smoke test。

## 这里应该放什么代码？

- `CoreTests`
- `MathTests`
- `AssetTests`
- `SceneTests`
- `RendererSmokeTests`

## 它依赖谁？

测试可以依赖 Engine。测试不应该被 Engine 依赖。

## 完成到什么程度算合格？

- `ctest --test-dir build` 可以运行。
- Core 生命周期有测试。
- Math 基础函数有测试。
- AssetManager 缓存有测试。
- Renderer smoke test 能跑几帧并关闭。

## 常见风险

- 渲染测试要求用户手动操作。
- 测试依赖绝对路径。
- 测试只覆盖成功路径，不覆盖失败路径。

## 文件与函数职责表

### 文件：`Tests/CoreTests.cpp`

- `TestApplicationLifecycleOrder()`
  - 作用：验证 Init、Run、Shutdown 顺序。
  - 输入：无。
  - 输出：测试通过/失败。
  - 调用时机：ctest。
  - 依赖：Core。
  - 验收标准：生命周期顺序与文档一致。
  - 常见风险：测试依赖真实窗口导致不稳定。

- `TestLayerAttachDetach()`
  - 作用：验证 LayerStack 添加和移除时回调正确。
  - 输入：无。
  - 输出：测试通过/失败。
  - 调用时机：ctest。
  - 依赖：Layer、LayerStack。
  - 验收标准：OnAttach/OnDetach 只调用一次。
  - 常见风险：未覆盖 overlay。

### 文件：`Tests/MathTests.cpp`

- `TestTransformMatrix()`
  - 作用：验证 Transform 矩阵。
  - 输入：固定位置、旋转、缩放。
  - 输出：测试通过/失败。
  - 调用时机：ctest。
  - 依赖：Math。
  - 验收标准：矩阵结果在误差范围内。
  - 常见风险：浮点比较使用精确相等。

- `TestAABBFrustumRay()`
  - 作用：验证 AABB、Frustum、Ray 基础行为。
  - 输入：固定测试数据。
  - 输出：测试通过/失败。
  - 调用时机：ctest。
  - 依赖：Math。
  - 验收标准：命中/不命中结果符合预期。
  - 常见风险：边界条件未覆盖。

### 文件：`Tests/AssetTests.cpp`

- `TestAssetCacheHit()`
  - 作用：验证同一路径资源重复加载命中缓存。
  - 输入：测试资源路径。
  - 输出：测试通过/失败。
  - 调用时机：ctest。
  - 依赖：AssetManager。
  - 验收标准：两次 handle 相同。
  - 常见风险：依赖不存在的本地资源。

- `TestMissingAssetFailure()`
  - 作用：验证缺失资源能优雅失败。
  - 输入：不存在的路径。
  - 输出：测试通过/失败。
  - 调用时机：ctest。
  - 依赖：AssetManager、Log。
  - 验收标准：返回无效 handle 且不崩溃。
  - 常见风险：错误路径导致测试污染日志。

### 文件：`Tests/RendererSmokeTests.cpp`

- `TestCreateWindowClearAndClose()`
  - 作用：创建窗口、清屏、运行几帧并关闭。
  - 输入：测试窗口配置。
  - 输出：测试通过/失败。
  - 调用时机：可选 smoke test。
  - 依赖：Platform、RHI/OpenGL、Renderer。
  - 验收标准：无崩溃、无严重 OpenGL 错误。
  - 常见风险：CI 或 WSLg 环境没有图形能力。

- `TestDrawIndexedTriangle()`
  - 作用：验证最小 draw indexed 路径。
  - 输入：测试三角形 mesh。
  - 输出：测试通过/失败。
  - 调用时机：Renderer 基础完成后。
  - 依赖：RHI、Renderer。
  - 验收标准：draw call 成功执行。
  - 常见风险：像素级验证过早引入不稳定性。
