# QA 与测试计划

本文说明如何验证引擎功能。测试目标不是追求一开始覆盖所有代码，而是每个阶段都有明确验收，防止架构越写越散。

## 测试分类

- 构建测试。
- Core 生命周期测试。
- Math 测试。
- AssetManager 测试。
- Renderer smoke test。
- Scene 测试。
- Sandbox 手动验收。

## 构建测试

命令：

```bash
cmake -S . -B build -G Ninja
cmake --build build
ctest --test-dir build
```

通过标准：

- CMake 配置成功。
- 编译成功。
- 测试目标可运行。
- `compile_commands.json` 存在。

## Core 生命周期测试

验证内容：

- `Application::Init` 被调用一次。
- `Application::Run` 能进入主循环。
- `Application::Shutdown` 被调用一次。
- Shutdown 顺序与 Init 相反。

异常场景：

- 初始化失败。
- 空 LayerStack。
- 连续启动和关闭。

## Math 测试

验证内容：

- Transform matrix。
- AABB 包围盒。
- Ray intersection。
- Frustum plane。

通过标准：

- 数值误差在可接受范围内。
- 坐标系约定一致。
- 矩阵乘法顺序明确。

## AssetManager 测试

验证内容：

- 重复加载同一路径命中缓存。
- 缺失资源输出错误。
- Shutdown 释放资源。
- 路径规范化。

通过标准：

- 同一资源只创建一次。
- 错误日志包含资源路径。
- Shutdown 不崩溃。

## Renderer Smoke Test

验证内容：

- 创建窗口。
- 创建 RHI device。
- 清屏。
- 绘制一个三角形或 fullscreen pass。
- 运行数帧后关闭。

通过标准：

- 无崩溃。
- OpenGL debug output 没有严重错误。
- 关闭时资源释放顺序正确。

## Scene 测试

验证内容：

- Entity 创建和销毁。
- Transform 组件。
- MeshRenderer 组件。
- Camera 组件。
- Light 组件。

通过标准：

- 删除 Entity 后组件同步删除。
- Scene 可以提交 render packet。
- 空 Scene 可以运行。

## Sandbox 手动验收

检查项：

- 窗口能打开。
- 相机能移动。
- 场景有多个物体。
- 材质有可见差异。
- Tone Mapping 可见。
- 关闭不崩溃。

## 回归测试节奏

每完成一个阶段：

```text
构建
-> 单元测试
-> Sandbox 运行
-> 检查日志
-> 记录问题
```

## 常见失败原因

- 生命周期顺序错误。
- GPU 资源在 Context 销毁后释放。
- Shader 路径错误。
- 资源重复加载。
- Scene 和 Renderer 互相持有不该持有的对象。
