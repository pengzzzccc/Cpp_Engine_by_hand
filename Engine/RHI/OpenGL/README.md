# Engine/RHI/OpenGL

`RHI/OpenGL` 是 RHI 的 OpenGL 实现。

## 这个文件夹干什么？

把 RHI 抽象转换成 OpenGL 调用，负责真实 GPU 资源创建、命令执行和错误检查。

## 这里应该放什么代码？

- `OpenGLRenderDevice`
- `OpenGLBuffer`
- `OpenGLTexture`
- `OpenGLShader`
- `OpenGLPipeline`
- `OpenGLFramebuffer`
- `OpenGLCommandList`
- `OpenGLUtils`

## 它依赖谁？

依赖 `Engine/RHI`、`Engine/Platform`、`Engine/Core` 和 glad/OpenGL。

## 完成到什么程度算合格？

- glad 初始化成功。
- 可以清屏和绘制 indexed mesh。
- Shader 编译错误会输出日志。
- GPU 资源在 Context 有效时释放。

## 常见风险

- 在 Context 创建前调用 OpenGL。
- 析构顺序导致 OpenGL object 泄漏或崩溃。
- OpenGL enum 转换散落在各处。
