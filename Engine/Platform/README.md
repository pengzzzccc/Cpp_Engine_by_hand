# Engine/Platform

`Platform` 是平台抽象层。

## 这个文件夹干什么？

封装 SDL2 窗口、输入、平台事件和 OpenGL Context 创建边界。

## 这里应该放什么代码？

- `Window`
- `SDLWindow`
- `Input`
- `InputState`
- `PlatformEvent`
- 平台文件系统入口

## 它依赖谁？

依赖 `Engine/Core`。不依赖 Renderer、Scene、Assets。

## 完成到什么程度算合格？

- 能创建和销毁窗口。
- 能处理关闭事件。
- 能读取键盘和鼠标状态。
- 能为 OpenGL 后端提供有效 Context。

## 常见风险

- 让 Scene 直接调用 SDL2。
- 输入状态没有按帧刷新。
- Context 生命周期和 Renderer 生命周期顺序混乱。

## 文件与函数职责表

### 文件：`Engine/Platform/Window.h`

职责：声明跨平台窗口接口。

- `Window::Create(const WindowDesc&)`
  - 作用：创建窗口实例。
  - 输入：窗口标题、宽高、是否启用 OpenGL。
  - 输出：窗口对象或成功/失败状态。
  - 调用时机：Application 初始化 Platform 时。
  - 依赖：具体平台实现，例如 SDLWindow。
  - 验收标准：Sandbox 能创建 1280x720 窗口。
  - 常见风险：直接返回裸指针导致所有权不清。

- `Window::PollEvents()`
  - 作用：轮询平台事件并更新输入状态。
  - 输入：无。
  - 输出：无。
  - 调用时机：每帧开始。
  - 依赖：SDL2 事件系统。
  - 验收标准：窗口关闭和输入事件能被捕获。
  - 常见风险：事件轮询不及时导致窗口无响应。

- `Window::SwapBuffers()`
  - 作用：交换前后缓冲，显示当前帧。
  - 输入：无。
  - 输出：无。
  - 调用时机：Renderer 完成一帧后。
  - 依赖：有效 OpenGL Context。
  - 验收标准：渲染画面能显示。
  - 常见风险：Context 无效时调用。

- `Window::ShouldClose()`
  - 作用：查询窗口是否请求关闭。
  - 输入：无。
  - 输出：`bool`。
  - 调用时机：Application 主循环条件或事件处理后。
  - 依赖：窗口事件状态。
  - 验收标准：点击关闭按钮后返回 true。
  - 常见风险：关闭事件未写入状态。

- `Window::GetWidth()` / `Window::GetHeight()`
  - 作用：获取当前窗口尺寸。
  - 输入：无。
  - 输出：像素宽高。
  - 调用时机：创建 framebuffer、更新相机 projection。
  - 依赖：窗口状态。
  - 验收标准：窗口 resize 后尺寸正确。
  - 常见风险：高 DPI 下逻辑尺寸和 framebuffer 尺寸混淆。

### 文件：`Engine/Platform/SDLWindow.h` / `SDLWindow.cpp`

职责：用 SDL2 实现 `Window`。

- `SDLWindow::Init(const WindowDesc&)`
  - 作用：初始化 SDL、创建窗口和 OpenGL Context。
  - 输入：窗口描述。
  - 输出：成功/失败状态。
  - 调用时机：`Window::Create` 内部。
  - 依赖：SDL2、OpenGL Context 参数。
  - 验收标准：窗口能打开，Context 可被 RHI/OpenGL 使用。
  - 常见风险：SDL 初始化和 Context 创建顺序错误。

- `SDLWindow::Shutdown()`
  - 作用：销毁 Context、窗口并退出 SDL 视频子系统。
  - 输入：无。
  - 输出：无。
  - 调用时机：Application 关闭 Platform 时。
  - 依赖：已创建的 SDL 资源。
  - 验收标准：关闭时不崩溃。
  - 常见风险：Renderer GPU 资源还没释放就销毁 Context。

- `SDLWindow::HandleEvent(const SDL_Event&)`
  - 作用：把 SDL 事件转换成引擎事件和输入状态。
  - 输入：SDL 事件。
  - 输出：无或引擎事件。
  - 调用时机：`PollEvents()` 内部。
  - 依赖：Input 模块。
  - 验收标准：键盘、鼠标、窗口关闭事件都能转换。
  - 常见风险：事件类型漏处理。

### 文件：`Engine/Platform/Input.h` / `Input.cpp`

职责：维护每帧输入状态。

- `Input::Update()`
  - 作用：在每帧开始时刷新上一帧和当前帧状态。
  - 输入：无。
  - 输出：无。
  - 调用时机：事件轮询前或后，需在设计中固定。
  - 依赖：Platform 事件。
  - 验收标准：`WasKeyPressed` 只在按下那一帧为 true。
  - 常见风险：刷新顺序错误导致按键边沿丢失。

- `Input::IsKeyDown(KeyCode)`
  - 作用：查询按键当前是否按下。
  - 输入：按键枚举。
  - 输出：`bool`。
  - 调用时机：相机移动、控制逻辑。
  - 依赖：当前键盘状态。
  - 验收标准：按住 WASD 时持续返回 true。
  - 常见风险：键码和 SDL key/scancode 混用。

- `Input::WasKeyPressed(KeyCode)` / `Input::WasKeyReleased(KeyCode)`
  - 作用：查询本帧按下或释放。
  - 输入：按键枚举。
  - 输出：`bool`。
  - 调用时机：一次性动作，例如跳跃、截图、切换模式。
  - 依赖：当前帧和上一帧状态。
  - 验收标准：一次按键只触发一次。
  - 常见风险：按住时每帧重复触发。

- `Input::GetMousePosition()` / `Input::GetMouseDelta()`
  - 作用：获取鼠标位置和本帧移动量。
  - 输入：无。
  - 输出：二维向量。
  - 调用时机：相机旋转、拾取、UI。
  - 依赖：鼠标事件。
  - 验收标准：鼠标移动能驱动相机转向。
  - 常见风险：窗口获得焦点前 delta 异常过大。
