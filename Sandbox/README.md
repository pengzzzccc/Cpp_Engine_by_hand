# Sandbox

`Sandbox` 是引擎示例程序。

## 这个文件夹干什么？

用真实 demo 场景验证引擎 API 是否好用。

## 这里应该放什么代码？

- `SandboxApp`
- `DemoScene`
- 自由相机控制。
- 示例 Mesh、Material、Light 创建。
- 调试开关。

## 它依赖谁？

依赖 Engine。Engine 不能依赖 Sandbox。

## 完成到什么程度算合格？

- 可以打开窗口。
- 可以渲染 3D 场景。
- 可以移动相机。
- 可以展示材质、灯光、Tone Mapping。
- 关闭时不崩溃。

## 常见风险

- 把 OpenGL 细节写进 Sandbox。
- 把临时 demo 代码当成 Engine 核心。
- 依赖绝对资源路径。
