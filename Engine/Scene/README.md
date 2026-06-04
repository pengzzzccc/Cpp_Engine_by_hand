# Engine/Scene

`Scene` 是场景和对象系统。

## 这个文件夹干什么？

管理 Entity 和 Component，并把场景数据转换成 Renderer 可以使用的 render packet。

## 这里应该放什么代码？

- `Scene`
- `Entity`
- `Component`
- `TransformComponent`
- `CameraComponent`
- `MeshRendererComponent`
- `LightComponent`
- `NameComponent`
- `HierarchyComponent`

## 它依赖谁？

依赖 Core、Math、Renderer、Assets。不依赖 Platform，不直接依赖 RHI/OpenGL。

## 完成到什么程度算合格？

- 可以创建和删除 Entity。
- Entity 可以挂载 Transform、Camera、MeshRenderer、Light。
- Scene 可以向 Renderer 提交可绘制对象。
- 空 Scene 可以正常运行。

## 常见风险

- 第一版 ECS 做得过度复杂。
- Entity 删除后组件残留。
- Scene 直接上传 GPU 资源。
