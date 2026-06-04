# third_party

`third_party` 放第三方库源码或 vendored 依赖说明。

## 这个文件夹干什么？

管理项目需要的外部库，避免依赖来源不清。

## 这里应该放什么代码或内容？

这里可以放 vendored 第三方源码、第三方库的 CMake 接入文件、license 说明和版本记录。不要把自己写的引擎模块代码放在这里。

## 建议依赖

- glad：OpenGL 函数加载器。
- GLM：数学库。
- stb_image：图片加载。
- ImGui：后续工具面板。
- Assimp：后续模型导入。

## 它依赖谁？

第三方库不应该依赖项目代码。项目模块可以通过 CMake 链接或 include 它们。

## 完成到什么程度算合格？

- 每个第三方库来源清楚。
- license 可追踪。
- CMake 接入方式明确。
- 不提交无关示例和大型临时文件。

## 常见风险

- 混用系统包和 vendored 包导致版本混乱。
- 忘记记录 license。
- 第三方 include 路径污染整个项目。
