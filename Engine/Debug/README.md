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

## 文件与函数职责表

### 文件：`Engine/Debug/DebugDraw.h` / `DebugDraw.cpp`

职责：收集并绘制调试几何。

- `DebugDraw::Line(start, end, color)`
  - 作用：添加一条调试线。
  - 输入：起点、终点、颜色。
  - 输出：无。
  - 调用时机：碰撞、相机、光线、包围盒调试。
  - 依赖：Renderer。
  - 验收标准：本帧能看到对应线段。
  - 常见风险：调试数据跨帧残留。

- `DebugDraw::Sphere(center, radius, color)` / `Box(aabb, color)`
  - 作用：添加线框球或盒子。
  - 输入：位置、尺寸、颜色。
  - 输出：无。
  - 调用时机：可视化包围体和拾取。
  - 依赖：Math、Renderer。
  - 验收标准：形状位置与世界坐标一致。
  - 常见风险：DebugDraw 使用不同坐标系。

- `DebugDraw::Flush(CommandList&)`
  - 作用：把本帧调试几何提交给 Renderer。
  - 输入：CommandList。
  - 输出：无。
  - 调用时机：Debug pass 执行时。
  - 依赖：RHI、Renderer。
  - 验收标准：Flush 后清空本帧队列。
  - 常见风险：Flush 中创建大量临时 GPU 资源。

### 文件：`Engine/Debug/Profiler.h` / `Profiler.cpp`

- `Profiler::BeginFrame()` / `EndFrame()`
  - 作用：开始和结束一帧性能采样。
  - 输入：无。
  - 输出：无。
  - 调用时机：Application 每帧开头和结尾。
  - 依赖：Time。
  - 验收标准：能得到 frame time。
  - 常见风险：采样本身影响性能判断。

- `Profiler::BeginScope(name)` / `EndScope()`
  - 作用：记录一个代码区间耗时。
  - 输入：scope 名称。
  - 输出：无。
  - 调用时机：渲染 Pass、资源加载、Scene 更新。
  - 依赖：Time、栈结构。
  - 验收标准：嵌套 scope 顺序正确。
  - 常见风险：Begin/End 不成对。

### 文件：`Engine/Debug/GpuMarker.h`

- `GpuMarker::Push(name)` / `Pop()`
  - 作用：标记 GPU 调试区间。
  - 输入：marker 名称。
  - 输出：无。
  - 调用时机：RenderPass 执行前后。
  - 依赖：RHI/OpenGL debug marker 支持。
  - 验收标准：调试工具或日志能看到 Pass 名称。
  - 常见风险：平台不支持时没有 fallback。
