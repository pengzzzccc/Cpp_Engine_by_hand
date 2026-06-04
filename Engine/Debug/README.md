# Engine/Debug

`Debug` 是调试辅助模块。

## 这个文件夹干什么？

提供 Debug Draw、Profiler、GPU marker 和日志辅助，让引擎问题更容易定位。

## 这里应该放什么代码？

- `DebugDraw`
- `Profiler`
- `ScopedTimer`
- `GpuMarker`
- Debug overlay 数据结构

## 它依赖谁？

可以依赖 Core 和 Renderer。不要让 Runtime 核心强依赖 Debug。

## 完成到什么程度算合格？

- 能绘制调试线框和辅助线。
- 能记录 CPU frame time。
- 能在日志中标记渲染 Pass。
- 关闭 Debug 不影响正常运行。

## 常见风险

- Debug 代码污染业务逻辑。
- Profiler 本身造成明显性能干扰。
- Debug GPU 资源生命周期不清。
