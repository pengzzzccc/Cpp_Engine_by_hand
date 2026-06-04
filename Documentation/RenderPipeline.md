# 现代渲染管线规划

本文描述 v0.1 的渲染管线。目标是用现代架构组织渲染流程，但底层先用 OpenGL 实现。

## 总体目标

```text
Scene
-> RenderPacket
-> RenderGraph
-> GBuffer Pass
-> Lighting Pass
-> ToneMapping Pass
-> Present
```

Renderer 不直接调用 OpenGL。Renderer 通过 RHI 创建资源和提交命令。OpenGL 后端负责把 RHI 命令翻译成 OpenGL 调用。

## RHI

RHI 是 Renderer 和图形 API 之间的接口。

v0.1 需要的 RHI 对象：

- `RenderDevice`
- `Buffer`
- `Texture`
- `Shader`
- `Pipeline`
- `Framebuffer`
- `Sampler`
- `CommandList`

RHI 描述的是“我要做什么”，不是“OpenGL 怎么做”。

## OpenGL 后端

OpenGL 后端负责：

- 初始化 glad。
- 创建 GPU resource。
- 编译 Shader。
- 创建 Framebuffer。
- 执行 draw call。
- 捕获 OpenGL 错误。

后端不应该知道 Scene、Entity、Component。

## RenderGraph

RenderGraph 负责组织一帧里的 Pass。

v0.1 先实现简单版本：

- 手动添加 Pass。
- 手动声明读写资源。
- 按添加顺序执行。
- 每帧清理临时资源。

未来扩展：

- 自动排序。
- 自动资源别名。
- 自动 barrier。
- Pass 裁剪。

## GBuffer Pass

输入：

- Mesh render packets。
- Camera matrices。
- Material。

输出：

- Albedo/Material texture。
- Normal texture。
- Depth texture。

职责：

- 绘制不透明几何体。
- 写入材质基础信息。
- 写入法线和深度。

验收：

- 可以单独查看 GBuffer。
- 多个物体能正确写入。
- 深度测试正确。

## Lighting Pass

输入：

- GBuffer textures。
- Light data。
- Camera data。

输出：

- HDR color texture。

职责：

- 基础 PBR 光照。
- Directional Light。
- 未来加入 Point Light、Spot Light、IBL。

验收：

- BaseColor、Metallic、Roughness 影响结果。
- 法线方向影响明暗。
- 光照输出到 HDR target。

## Shadow Pass

优先级：Should

输入：

- Light view/projection。
- Shadow caster mesh。

输出：

- Shadow map。

职责：

- 为 Directional Light 生成阴影。

v0.1 可以先规划接口，后续实现。

## Transparent / Forward Pass

优先级：Could

Deferred 不适合所有透明物体。未来可以加入 Forward Transparent Pass：

- 粒子。
- 透明材质。
- Debug geometry。

v0.1 可以先不实现。

## PostProcess

v0.1 必须包含：

- Tone Mapping。
- Gamma correction。

Should：

- Bloom。
- Exposure。

Could：

- Color grading。
- FXAA。

## Present

Present 负责把最终 LDR color 输出到窗口 backbuffer。

注意：

- Present 不应该混入 Lighting 逻辑。
- Present 不应该创建 Scene 数据。
- Present 只做最终输出。

## 资源生命周期

持久资源：

- Mesh buffer。
- Texture。
- Shader。
- Material。

每帧资源：

- GBuffer。
- HDR target。
- 临时 postprocess target。

关闭顺序：

```text
RenderGraph temp resources
-> Renderer persistent resources
-> Asset GPU resources
-> RHI device
-> OpenGL context
```

## 最小可运行画面

v0.1 最小画面：

- 一个立方体。
- 一个地面。
- 一个 Directional Light。
- 一个自由相机。
- GBuffer Pass。
- Lighting Pass。
- ToneMapping Pass。

这就是现代管线的最小闭环。
