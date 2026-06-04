# Engine/Core

`Core` 是引擎的基础层。

## 这个文件夹干什么？

负责引擎生命周期、主循环、时间、日志、断言和基础事件系统。

## 这里应该放什么代码？

- `Application`
- `EngineContext`
- `Layer`
- `LayerStack`
- `Time`
- `Log`
- `Assert`
- `Event`

## 它依赖谁？

Core 不应该依赖任何其他引擎模块。其他模块可以依赖 Core。

## 完成到什么程度算合格？

- 程序能按 `Init -> Run -> Shutdown` 启动和关闭。
- 主循环能计算 delta time。
- 日志能记录模块初始化和关闭顺序。
- 没有高层模块反向进入 Core。

## 常见风险

- 把窗口、渲染或场景逻辑写进 Core。
- 使用全局变量控制生命周期。
- 析构和 Shutdown 责任不清。

## 文件与函数职责表

### 文件：`Engine/Core/Application.h` / `Application.cpp`

职责：声明和实现引擎应用入口，统一管理初始化、主循环、关闭和 Layer 调度。

- `Application::Init()`
  - 作用：初始化 Core 状态，并按顺序创建 Platform、RHI、Renderer、Assets、Scene 等系统。
  - 输入：无，后续可扩展为 `ApplicationDesc`。
  - 输出：建议返回 `bool` 表示初始化是否成功。
  - 调用时机：程序启动后、进入主循环前。
  - 依赖：Core 内部日志、时间系统；后续会调度 Platform、Renderer、Assets。
  - 验收标准：日志能看到清楚的初始化顺序。
  - 常见风险：构造函数里做初始化，导致失败时无法优雅退出。

- `Application::Run()`
  - 作用：执行主循环，驱动事件轮询、Layer 更新、渲染和窗口交换。
  - 输入：无。
  - 输出：无。
  - 调用时机：`Init()` 成功后。
  - 依赖：`Time`、`LayerStack`、Platform 事件、Renderer。
  - 验收标准：窗口关闭或 `RequestClose()` 后能退出循环。
  - 常见风险：循环里没有 delta time，导致逻辑与帧率绑定。

- `Application::Shutdown()`
  - 作用：按初始化的相反顺序释放 Scene、Assets、Renderer、RHI、Platform 和 Core。
  - 输入：无。
  - 输出：无。
  - 调用时机：主循环结束后。
  - 依赖：所有已初始化模块。
  - 验收标准：关闭日志顺序清楚，没有重复释放。
  - 常见风险：OpenGL Context 销毁后才释放 GPU 资源。

- `Application::RequestClose()`
  - 作用：请求主循环结束。
  - 输入：无。
  - 输出：无。
  - 调用时机：窗口关闭事件、测试或 Sandbox 主动退出时。
  - 依赖：`m_Running` 或类似运行状态。
  - 验收标准：下一帧或当前事件处理结束后退出主循环。
  - 常见风险：直接 `exit()`，跳过正常 Shutdown。

- `Application::IsRunning()`
  - 作用：查询主循环是否仍在运行。
  - 输入：无。
  - 输出：`bool`。
  - 调用时机：测试、调试或主循环条件判断。
  - 依赖：运行状态变量。
  - 验收标准：`RequestClose()` 后返回 false。
  - 常见风险：状态被多个模块随意修改。

- `Application::PushLayer(Layer*)` / `Application::PopLayer(Layer*)`
  - 作用：添加或移除普通 Layer。
  - 输入：Layer 指针或智能指针。
  - 输出：无。
  - 调用时机：初始化功能模块、切换 demo、测试时。
  - 依赖：`LayerStack`。
  - 验收标准：Layer 的 `OnAttach` / `OnDetach` 被正确调用。
  - 常见风险：Layer 所有权不清导致悬空指针。

### 文件：`Engine/Core/Layer.h`

职责：定义可挂载到应用主循环中的功能层。

- `Layer::OnAttach()`
  - 作用：Layer 被加入时初始化自己的资源。
  - 输入：无。
  - 输出：无。
  - 调用时机：`PushLayer` 或 `PushOverlay` 时。
  - 依赖：具体 Layer 自己需要的模块。
  - 验收标准：只调用一次。
  - 常见风险：在这里创建但不在 `OnDetach` 释放。

- `Layer::OnDetach()`
  - 作用：Layer 被移除时释放资源。
  - 输入：无。
  - 输出：无。
  - 调用时机：`PopLayer`、`PopOverlay` 或 Application 关闭时。
  - 依赖：Layer 持有的资源。
  - 验收标准：资源释放顺序清楚。
  - 常见风险：重复 detach。

- `Layer::OnUpdate(float deltaTime)`
  - 作用：更新逻辑。
  - 输入：本帧 delta time，单位秒。
  - 输出：无。
  - 调用时机：每帧事件处理后、渲染前。
  - 依赖：Time 和 Layer 自己的数据。
  - 验收标准：帧率变化时运动速度稳定。
  - 常见风险：使用固定帧时间。

- `Layer::OnRender()`
  - 作用：提交渲染相关工作。
  - 输入：无，后续可接收 frame context。
  - 输出：无。
  - 调用时机：每帧更新后。
  - 依赖：Renderer。
  - 验收标准：Layer 能提交自己的渲染数据。
  - 常见风险：直接调用 OpenGL。

- `Layer::OnEvent(const Event&)`
  - 作用：接收窗口、输入和系统事件。
  - 输入：事件对象。
  - 输出：是否消费事件，建议返回 `bool`。
  - 调用时机：Platform 轮询事件后。
  - 依赖：Event 系统。
  - 验收标准：关闭事件、输入事件能被 Layer 响应。
  - 常见风险：事件传播顺序不清。

### 文件：`Engine/Core/LayerStack.h` / `LayerStack.cpp`

职责：维护普通 Layer 和 Overlay 的顺序。

- `LayerStack::PushLayer(Layer*)`
  - 作用：加入普通 Layer。
  - 输入：Layer 指针或智能指针。
  - 输出：无。
  - 调用时机：Application 初始化或运行时加载功能。
  - 依赖：Layer。
  - 验收标准：普通 Layer 插入到 Overlay 前。
  - 常见风险：插入顺序导致 UI 或工具层被覆盖。

- `LayerStack::PushOverlay(Layer*)`
  - 作用：加入 Overlay，例如 ImGui 工具层。
  - 输入：Layer 指针或智能指针。
  - 输出：无。
  - 调用时机：工具层或调试层初始化时。
  - 依赖：Layer。
  - 验收标准：Overlay 总在普通 Layer 之后更新/渲染。
  - 常见风险：Overlay 与普通 Layer 生命周期混淆。

- `LayerStack::PopLayer(Layer*)` / `LayerStack::PopOverlay(Layer*)`
  - 作用：移除 Layer 并触发 detach。
  - 输入：要移除的 Layer。
  - 输出：是否成功移除。
  - 调用时机：模块卸载或关闭时。
  - 依赖：Layer 容器。
  - 验收标准：不存在重复 Layer 或悬空 Layer。
  - 常见风险：遍历时删除导致迭代器失效。

- `LayerStack::Begin()` / `LayerStack::End()`
  - 作用：提供遍历接口。
  - 输入：无。
  - 输出：迭代器。
  - 调用时机：Application 每帧更新、渲染、事件分发时。
  - 依赖：内部容器。
  - 验收标准：可按顺序遍历全部 Layer。
  - 常见风险：外部持有迭代器后修改容器。

### 文件：`Engine/Core/Time.h`

职责：提供时间点、帧间隔和主循环计时。

- `Time::Now()`
  - 作用：返回当前高精度时间点。
  - 输入：无。
  - 输出：时间点或秒数。
  - 调用时机：帧开始、性能统计。
  - 依赖：`std::chrono`。
  - 验收标准：连续调用时间单调递增。
  - 常见风险：混用毫秒和秒。

- `Time::UpdateFrameTime()`
  - 作用：计算上一帧到当前帧的 delta time。
  - 输入：无。
  - 输出：无，内部更新 delta。
  - 调用时机：每帧开头。
  - 依赖：上一帧时间点。
  - 验收标准：delta time 合理且非负。
  - 常见风险：第一帧 delta 异常过大。

- `Time::DeltaTime()`
  - 作用：获取本帧 delta time。
  - 输入：无。
  - 输出：float 或 double，单位秒。
  - 调用时机：Layer 更新、相机移动、动画更新。
  - 依赖：`UpdateFrameTime()` 已执行。
  - 验收标准：同一帧内返回值稳定。
  - 常见风险：未更新就读取。

### 文件：`Engine/Core/Log.h`

职责：提供统一日志入口。

- `Log::Init()` / `Log::Shutdown()`
  - 作用：初始化和关闭日志系统。
  - 输入：无，后续可接收日志配置。
  - 输出：无。
  - 调用时机：Application 最早初始化、最后关闭。
  - 依赖：标准输出或日志库。
  - 验收标准：日志系统可用且关闭后不再输出。
  - 常见风险：模块关闭后仍写日志。

- `Log::Info()` / `Log::Warn()` / `Log::Error()` / `Log::Debug()`
  - 作用：输出不同级别日志。
  - 输入：消息字符串和可选参数。
  - 输出：无。
  - 调用时机：初始化、错误处理、资源加载、调试。
  - 依赖：日志系统已初始化。
  - 验收标准：输出包含级别和模块信息。
  - 常见风险：错误日志缺少上下文。

### 文件：`Engine/Core/Assert.h`

职责：提供运行时断言和验证宏。

- `ENGINE_ASSERT(condition, message)`
  - 作用：开发期检查必须成立的条件。
  - 输入：条件表达式和错误消息。
  - 输出：条件失败时中断或记录错误。
  - 调用时机：检测非法状态、越界、空指针。
  - 依赖：Log 和编译配置。
  - 验收标准：Debug 下能定位失败文件和行号。
  - 常见风险：Release 下副作用表达式被移除。

- `ENGINE_VERIFY(condition, message)`
  - 作用：检查条件并保留表达式执行。
  - 输入：条件表达式和错误消息。
  - 输出：失败时记录错误，可返回 bool。
  - 调用时机：必须执行的 API 调用和资源加载。
  - 依赖：Log。
  - 验收标准：Release 下仍执行条件表达式。
  - 常见风险：和 Assert 混用导致行为不一致。
