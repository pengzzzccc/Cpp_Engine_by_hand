# Engine

`Engine` 是引擎源码根目录，所有运行时模块都放在这里。

## 这个文件夹干什么？

组织引擎核心代码，提供给 `Sandbox` 和未来工具使用。

## 这里应该放什么代码？

- Core 生命周期。
- Platform 窗口和输入。
- Math 辅助。
- RHI 抽象和 OpenGL 后端。
- Renderer。
- Scene。
- Assets。
- Debug。
- Tools。

## 它依赖谁？

`Engine` 内部模块按单向依赖组织。`Engine` 作为整体可以依赖 `third_party`，但不能依赖 `Sandbox`。

## 完成到什么程度算合格？

- `Engine` 可以被 CMake 构建成库。
- `Sandbox` 能链接并使用 `Engine`。
- 每个子模块职责清晰。
- 高层模块不 include 低层实现细节。
