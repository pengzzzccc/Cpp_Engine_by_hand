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
