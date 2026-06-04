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
