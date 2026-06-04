# 术语表

## RHI

Render Hardware Interface，渲染硬件接口。它把 Renderer 和具体图形 API 隔开，让上层不直接依赖 OpenGL、Vulkan 或 DirectX。

## RenderGraph

描述一帧渲染 Pass 和资源读写关系的结构。它让渲染流程更清晰，也方便未来做资源复用、Pass 裁剪和调试可视化。

## FrameGraph

和 RenderGraph 类似，强调一帧内的渲染流程组织。本文中可以把 FrameGraph 理解为 RenderGraph 的同类概念。

## GBuffer

Geometry Buffer。Deferred Rendering 中用于存储几何和材质信息的多个 render target，例如 albedo、normal、depth、roughness。

## Deferred Rendering

延迟渲染。先把几何信息写入 GBuffer，再统一计算光照。适合多光源场景，但透明物体处理更复杂。

## Forward Rendering

前向渲染。绘制物体时直接计算光照。实现简单，透明物体更自然，但大量光源时成本较高。

## PBR

Physically Based Rendering，基于物理的渲染。用 BaseColor、Metallic、Roughness 等参数模拟更稳定的材质表现。

## HDR

High Dynamic Range，高动态范围。渲染中允许颜色超过 0 到 1，再通过 Tone Mapping 转换到屏幕范围。

## Tone Mapping

把 HDR 颜色转换成屏幕能显示的 LDR 颜色。常见方法有 Reinhard、ACES 等。

## CommandList

渲染命令列表。用于收集和提交绘制命令，例如 bind pipeline、bind buffer、draw indexed。

## Pipeline

渲染管线状态集合，包括 Shader、深度测试、混合、剔除、顶点布局等。

## Framebuffer

渲染目标集合。OpenGL 中 framebuffer 可以绑定多个 color attachment 和 depth attachment。

## Shader

运行在 GPU 上的小程序。顶点 Shader 处理顶点，片段 Shader 处理像素颜色。

## Mesh

网格。由顶点、索引和子网格组成，用来描述可绘制几何体。

## Material

材质。描述表面如何响应光照，通常引用 Shader、Texture 和参数。

## Asset

资源。包括 Shader、Texture、Mesh、Material、Scene 等磁盘文件和运行时对象。

## Entity

场景中的对象 ID。Entity 本身通常不包含大量逻辑，而是通过 Component 组合能力。

## Component

组件。挂在 Entity 上的数据或能力，例如 Transform、Camera、MeshRenderer、Light。

## Transform

位置、旋转、缩放。决定对象在世界中的空间状态。

## Frustum Culling

视锥裁剪。判断物体是否在相机可见范围内，不可见则跳过渲染。

## Smoke Test

烟雾测试。只验证功能是否能基本跑起来，不追求完整精确验证。
