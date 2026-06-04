# Engine/Renderer

`Renderer` 是渲染系统的高层模块。

## 这个文件夹干什么？

负责一帧渲染流程、RenderGraph、Deferred GBuffer、Lighting、PostProcess 和 Debug Draw。

## 这里应该放什么代码？

- `Renderer`
- `FrameRenderer`
- `RenderGraph`
- `RenderPass`
- `RenderResource`
- `Mesh`
- `Material`
- `Camera`
- `GBufferPass`
- `LightingPass`
- `ToneMappingPass`
- `DebugDraw`

## 它依赖谁？

依赖 RHI、Assets、Math、Core。不要直接依赖 OpenGL 或 SDL2。

## 完成到什么程度算合格？

- 能通过 RHI 提交绘制。
- 能执行 GBuffer、Lighting、ToneMapping、Present。
- 能渲染一个有材质的 3D 场景。
- Pass 顺序和资源关系可追踪。

## 常见风险

- Renderer 直接写 OpenGL。
- RenderGraph 过度复杂。
- Scene 和 Renderer 互相持有强依赖。
