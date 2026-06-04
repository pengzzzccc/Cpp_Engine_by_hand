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
