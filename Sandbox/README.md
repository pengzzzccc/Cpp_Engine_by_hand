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

## 文件与函数职责表

### 文件：`Sandbox/SandboxApp.h` / `SandboxApp.cpp`

职责：定义示例应用入口。

- `SandboxApp::Create()`
  - 作用：创建 Sandbox 应用实例。
  - 输入：无。
  - 输出：Application 或 SandboxApp 实例。
  - 调用时机：程序 main 入口。
  - 依赖：Engine/Core。
  - 验收标准：能被 main 调用并启动。
  - 常见风险：Sandbox 创建 Engine 内部资源而不是调用公开 API。

- `SandboxApp::OnInit()`
  - 作用：加载 demo scene、资源和相机。
  - 输入：无。
  - 输出：成功/失败状态。
  - 调用时机：Application Init 后。
  - 依赖：Scene、Assets、Renderer。
  - 验收标准：场景资源加载成功。
  - 常见风险：使用绝对路径。

- `SandboxApp::OnUpdate(float deltaTime)`
  - 作用：更新相机和 demo 逻辑。
  - 输入：delta time。
  - 输出：无。
  - 调用时机：每帧。
  - 依赖：Input、Scene。
  - 验收标准：相机移动帧率无关。
  - 常见风险：把 Renderer 底层逻辑写在这里。

- `SandboxApp::OnShutdown()`
  - 作用：卸载 demo scene 和临时资源。
  - 输入：无。
  - 输出：无。
  - 调用时机：Application Shutdown 前后按设计固定。
  - 依赖：Scene、Assets。
  - 验收标准：关闭无崩溃。
  - 常见风险：资源释放顺序和 Engine 冲突。

### 文件：`Sandbox/DemoScene.h` / `DemoScene.cpp`

- `DemoScene::Load(AssetManager&, Scene&)`
  - 作用：创建 demo 实体、mesh、material、light 和 camera。
  - 输入：AssetManager、Scene。
  - 输出：成功/失败状态。
  - 调用时机：Sandbox 初始化。
  - 依赖：Scene、Assets。
  - 验收标准：场景包含多个可渲染对象。
  - 常见风险：直接创建 OpenGL 资源。

- `DemoScene::Update(float deltaTime)`
  - 作用：更新 demo 动画或展示逻辑。
  - 输入：delta time。
  - 输出：无。
  - 调用时机：每帧。
  - 依赖：Scene。
  - 验收标准：旋转物体或相机控制正常。
  - 常见风险：逻辑与帧率绑定。

- `DemoScene::Unload()`
  - 作用：销毁 demo entity，释放场景引用。
  - 输入：无。
  - 输出：无。
  - 调用时机：Sandbox 关闭或切换场景。
  - 依赖：Scene。
  - 验收标准：卸载后空场景可运行。
  - 常见风险：AssetManager 资源引用未释放。
