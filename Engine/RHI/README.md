# Engine/RHI

`RHI` 是 Render Hardware Interface，渲染硬件接口。

## 这个文件夹干什么？

定义 Renderer 使用的抽象渲染接口，让上层不用直接依赖 OpenGL。

## 这里应该放什么代码？

- `RenderDevice`
- `Buffer`
- `Texture`
- `Shader`
- `Pipeline`
- `Framebuffer`
- `Sampler`
- `CommandList`
- RHI 描述结构和枚举

## 它依赖谁？

可以依赖 Core 和 Math。不依赖 Scene，不包含 OpenGL 头文件。

## 完成到什么程度算合格？

- Renderer 可以只通过 RHI 创建资源和提交命令。
- OpenGL 后端可以完整实现 v0.1 接口。
- RHI 头文件没有图形 API 实现细节。

## 常见风险

- 抽象过度，第一版无法实现。
- 抽象不足，OpenGL 细节泄漏到 Renderer。
- RHI 认识 Entity 或 Component。
