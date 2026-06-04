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

## 文件与函数职责表

### 文件：`Engine/Scene/Scene.h` / `Scene.cpp`

职责：管理 Entity、Component 和场景级更新。

- `Scene::CreateEntity(const std::string& name)`
  - 作用：创建 Entity 并默认添加 Name/Transform。
  - 输入：实体名称。
  - 输出：Entity handle。
  - 调用时机：加载场景、Sandbox 创建对象。
  - 依赖：Entity 存储和组件容器。
  - 验收标准：新 Entity 可添加组件并被查询。
  - 常见风险：Entity ID 重复。

- `Scene::DestroyEntity(Entity entity)`
  - 作用：删除 Entity 及其所有组件。
  - 输入：Entity。
  - 输出：无。
  - 调用时机：场景卸载、对象销毁。
  - 依赖：组件容器。
  - 验收标准：删除后组件不残留。
  - 常见风险：Renderer 仍持有旧 Entity 引用。

- `Scene::Update(float deltaTime)`
  - 作用：更新场景逻辑和组件状态。
  - 输入：delta time。
  - 输出：无。
  - 调用时机：每帧渲染前。
  - 依赖：Core Time、组件数据。
  - 验收标准：动画、相机或脚本更新帧率无关。
  - 常见风险：直接在 Update 中提交 GPU 命令。

- `Scene::Render(Renderer&)`
  - 作用：把 Scene 数据提交给 Renderer。
  - 输入：Renderer。
  - 输出：无。
  - 调用时机：Scene Update 后。
  - 依赖：Renderer、MeshRenderer、Camera、Light。
  - 验收标准：场景物体能被渲染。
  - 常见风险：Scene 直接调用 OpenGL。

- `Scene::CollectRenderPackets()`
  - 作用：收集可渲染对象，生成 Renderer 可消费的数据包。
  - 输入：无。
  - 输出：render packet 列表。
  - 调用时机：Renderer BeginFrame 前。
  - 依赖：MeshRendererComponent、TransformComponent、CameraComponent。
  - 验收标准：多个 Entity 能生成多个 packet。
  - 常见风险：packet 持有组件裸引用导致悬空。

### 文件：`Engine/Scene/Entity.h`

职责：提供 Entity 的组件访问 API。

- `Entity::AddComponent<T>(args...)`
  - 作用：给 Entity 添加组件。
  - 输入：组件构造参数。
  - 输出：组件引用。
  - 调用时机：创建对象、加载场景。
  - 依赖：Scene 组件存储。
  - 验收标准：重复添加策略明确。
  - 常见风险：Entity 不属于当前 Scene。

- `Entity::GetComponent<T>()`
  - 作用：获取组件。
  - 输入：组件类型。
  - 输出：组件引用。
  - 调用时机：更新、渲染数据收集。
  - 依赖：组件存在。
  - 验收标准：缺失组件时能断言或安全处理。
  - 常见风险：返回悬空引用。

- `Entity::HasComponent<T>()`
  - 作用：判断组件是否存在。
  - 输入：组件类型。
  - 输出：`bool`。
  - 调用时机：条件逻辑和安全访问前。
  - 依赖：组件存储。
  - 验收标准：结果与 Add/Remove 一致。
  - 常见风险：组件删除后缓存状态未更新。

- `Entity::RemoveComponent<T>()`
  - 作用：移除组件。
  - 输入：组件类型。
  - 输出：无或成功状态。
  - 调用时机：对象能力变化、销毁前。
  - 依赖：组件存储。
  - 验收标准：移除后 `HasComponent` 为 false。
  - 常见风险：移除 Transform 导致对象不可用。

### 文件：`Engine/Scene/Components.h`

职责：定义组件数据结构。

- `TransformComponent`
  - 字段：Position、Rotation、Scale。
  - 作用：描述 Entity 的世界或局部变换。
  - 调用时机：Scene 更新、Renderer 收集 packet。
  - 验收标准：能生成 model matrix。
  - 常见风险：父子层级和世界变换混淆。

- `CameraComponent`
  - 字段：FOV、Near、Far、Aspect、Primary。
  - 作用：让 Entity 成为相机。
  - 调用时机：Renderer 选择 active camera。
  - 验收标准：Scene 至少能找到一个主相机。
  - 常见风险：多个 primary camera 未处理。

- `MeshRendererComponent`
  - 字段：MeshHandle、MaterialHandle。
  - 作用：让 Entity 可被渲染。
  - 调用时机：CollectRenderPackets。
  - 验收标准：mesh 和 material 有效时生成 packet。
  - 常见风险：资源 handle 已失效。

- `LightComponent`
  - 字段：Type、Color、Intensity、Direction。
  - 作用：描述灯光。
  - 调用时机：Lighting Pass 数据收集。
  - 验收标准：Directional Light 能影响画面。
  - 常见风险：灯光单位和 PBR 公式不匹配。

- `NameComponent` / `HierarchyComponent`
  - 作用：提供调试名称和父子关系。
  - 调用时机：工具面板、序列化、场景管理。
  - 验收标准：工具能显示对象名称。
  - 常见风险：父子删除不同步。

### 文件：`Engine/Scene/SceneRendererBridge.h`

- `SceneRendererBridge::BuildRenderPackets(Scene&)`
  - 作用：把 Scene 组件数据转换成 Renderer 数据。
  - 输入：Scene。
  - 输出：render packet 列表和 frame scene data。
  - 调用时机：每帧渲染前。
  - 依赖：Scene、Renderer 类型。
  - 验收标准：Renderer 不需要直接遍历组件存储。
  - 常见风险：桥接层变成第二套 Scene 系统。
