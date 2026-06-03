# Cpp_Engine_by_hand

从 0 开始，用 C++ 手写一个轻量 3D 游戏引擎。

这份 README 面向这样的你：

- 会一门或几门编程语言，知道变量、函数、条件、循环这些概念。
- 不要求会 C++。
- 不要求会 CMake。
- 不要求会 OpenGL。
- 不要求做过游戏引擎。
- 开发环境使用 WSL2 + Ubuntu 24.04。

我们不会一上来就写一个庞大的引擎。路线是先让程序跑起来，再一点点拆模块，最后得到一个可继续扩展的 v0.1 引擎。

最终目标：

- 使用 `C++17 + CMake + SDL2 + OpenGL` 搭建工程。
- 能打开窗口、处理键盘鼠标输入。
- 能用 OpenGL 清屏、画三角形、画旋转立方体。
- 能用键盘和鼠标移动 3D 相机。
- 能拆出 `Core`、`Platform`、`Renderer`、`Scene`、`Resource` 模块。
- 能管理 Scene、Entity、Transform、Mesh、Texture、Shader。
- 能清楚维护启动、更新、渲染、卸载和关闭生命周期。

## 1. 开发环境：WSL2 + Ubuntu 24.04

推荐在 WSL 的 Linux 文件系统里开发，例如：

```bash
mkdir -p ~/Code
cd ~/Code
git clone <your-repo-url> Cpp_Engine_by_hand
cd Cpp_Engine_by_hand
```

不建议把项目长期放在 `/mnt/c/...` 下面开发。Windows 文件系统和 WSL Linux 文件系统之间的性能、权限和换行处理都可能带来额外问题。

### 1.1 安装基础工具

```bash
sudo apt update
sudo apt install -y build-essential cmake ninja-build gdb git pkg-config
```

检查版本：

```bash
g++ --version
cmake --version
ninja --version
gdb --version
git --version
```

### 1.2 安装 SDL2 和 OpenGL 依赖

```bash
sudo apt install -y libsdl2-dev libgl1-mesa-dev libglu1-mesa-dev mesa-utils
```

后面会用到的库：

- `SDL2`：创建窗口、处理输入、创建 OpenGL 上下文。
- `OpenGL`：渲染 API。
- `glad`：OpenGL 函数加载器，后续加入项目。
- `GLM`：数学库，处理向量、矩阵、相机。
- `stb_image`：图片加载库。
- `Assimp`：后续扩展模型加载时再加入。

### 1.3 检查 WSLg 和 OpenGL

Windows 11 的 WSLg 通常可以直接显示 Linux GUI 窗口。先检查：

```bash
echo $DISPLAY
glxinfo -B
```

如果 `echo $DISPLAY` 没有输出，或者 `glxinfo -B` 报错，先排查 WSLg、显卡驱动和 OpenGL 环境。

可以运行一个 Mesa 示例：

```bash
glxgears
```

如果能看到窗口和齿轮动画，说明 WSL2 的图形环境基本可用。

## 2. 工具与库：作用、历史和本项目用途

这一节不是为了背概念，而是让你知道每个工具为什么会出现在这个项目里。先有地图，后面遇到报错时才不会像在黑箱里摸索。

### 2.1 环境工具

#### WSL2

- 它是什么：Windows Subsystem for Linux 2，让 Windows 可以运行真正的 Linux 内核环境。
- 简短历史：WSL1 更像系统调用兼容层，WSL2 改成轻量虚拟机和 Linux 内核，兼容性更好。
- 本项目中负责什么：提供 Ubuntu、Linux 编译工具链、SDL2/OpenGL 开发环境。
- 什么时候需要关心它：窗口打不开、文件放在 `/mnt/c/...` 很慢、Linux 和 Windows 路径混用时。

#### Ubuntu 24.04

- 它是什么：一个常用 Linux 发行版，24.04 是长期支持版。
- 简短历史：Ubuntu 从 Debian 发展而来，目标是提供更易用、更稳定的桌面和服务器 Linux。
- 本项目中负责什么：提供 `apt` 包管理、编译器、SDL2、Mesa、CMake 等依赖。
- 什么时候需要关心它：安装依赖、查包名、处理系统库版本问题时。

#### Bash / 终端

- 它是什么：Linux 常用命令行 Shell，负责执行 `cd`、`mkdir`、`cmake`、`git` 这些命令。
- 简短历史：Bash 是 Bourne shell 的增强版本，长期作为 GNU/Linux 默认 Shell 使用。
- 本项目中负责什么：运行安装、构建、测试、调试命令。
- 什么时候需要关心它：命令找不到、路径不对、当前目录不对、脚本执行失败时。

#### Git

- 它是什么：版本控制工具，用来记录代码变化、回退历史、管理分支。
- 简短历史：Git 由 Linus Torvalds 在 2005 年创建，最初是为了管理 Linux 内核源码。
- 本项目中负责什么：保存每一步引擎代码和 README 的演进。
- 什么时候需要关心它：提交代码、查看改动、回退自己的错误修改、同步远程仓库时。

### 2.2 编译和调试工具

#### build-essential

- 它是什么：Ubuntu 上的一组基础编译工具包。
- 简短历史：Debian/Ubuntu 用它作为 C/C++ 本地编译环境的常用入口包。
- 本项目中负责什么：安装 `g++`、`make`、标准库头文件等基础能力。
- 什么时候需要关心它：出现 `g++: command not found` 或标准头文件找不到时。

#### g++

- 它是什么：GNU C++ 编译器，把 `.cpp` 源码编译成目标文件和可执行程序。
- 简短历史：g++ 属于 GCC 项目，GCC 是 GNU 体系里最重要的编译器集合之一。
- 本项目中负责什么：真正编译引擎和 Sandbox 的 C++ 代码。
- 什么时候需要关心它：编译错误、链接错误、C++ 标准版本不匹配时。

#### C++17

- 它是什么：C++ 语言在 2017 年发布的标准版本。
- 简短历史：C++ 从 C with Classes 发展而来，经历 C++98、C++11、C++14、C++17 等标准演进。
- 本项目中负责什么：提供较现代但不过度复杂的语言能力，例如 `std::unique_ptr`、`std::filesystem`、结构化绑定等。
- 什么时候需要关心它：代码用了新语法但编译器没开启 `-std=c++17` 时。

#### CMake

- 它是什么：跨平台构建系统生成器，负责描述项目如何被构建。
- 简短历史：CMake 由 Kitware 在 2000 年前后创建，常用于 C/C++ 跨平台工程。
- 本项目中负责什么：组织 `Engine` 静态库、`Sandbox` 可执行程序、include 路径和依赖链接。
- 什么时候需要关心它：新增文件、新增库、include 路径错误、链接 SDL2/OpenGL 失败时。

#### Ninja

- 它是什么：一个小而快的构建工具，执行 CMake 生成的构建任务。
- 简短历史：Ninja 由 Evan Martin 创建，目标是比传统 Make 更快、更适合自动生成。
- 本项目中负责什么：实际执行编译命令，生成最终的 `Sandbox`。
- 什么时候需要关心它：`cmake --build build` 很慢、构建文件损坏、需要清理 build 目录时。

#### pkg-config

- 它是什么：查询系统库编译参数和链接参数的工具。
- 简短历史：它常用于 Unix/Linux C/C++ 生态，解决“库装了但编译器不知道怎么链接”的问题。
- 本项目中负责什么：帮助 CMake 或手写命令找到 SDL2 等库的 include 和 link 参数。
- 什么时候需要关心它：`sdl2-config` 或 CMake 找不到 SDL2，或者链接参数缺失时。

#### GDB

- 它是什么：GNU 调试器，可以断点、单步、查看调用栈和变量。
- 简短历史：GDB 是 GNU 工具链的重要组成部分，长期用于 C/C++ 程序调试。
- 本项目中负责什么：排查崩溃、空指针、生命周期顺序错误。
- 什么时候需要关心它：程序段错误、窗口关闭崩溃、资源释放后仍被访问时。

### 2.3 图形、窗口和 OpenGL 工具

#### SDL2

- 它是什么：跨平台多媒体库，常用于创建窗口、处理输入、音频和 OpenGL 上下文。
- 简短历史：SDL 最早由 Sam Lantinga 创建，SDL2 在 2013 年发布，是很多游戏和工具的底层库。
- 本项目中负责什么：创建 WSLg 下的窗口，处理键盘鼠标，创建 OpenGL 上下文。
- 什么时候需要关心它：窗口打不开、输入没响应、OpenGL 上下文创建失败时。

#### OpenGL

- 它是什么：跨平台图形 API，用来把顶点、纹理、Shader 渲染到屏幕。
- 简短历史：OpenGL 由 SGI 在 1992 年推出，长期是图形编程和教学中最常见的 API 之一。
- 本项目中负责什么：清屏、绘制三角形、绘制立方体、处理深度测试和 Shader。
- 什么时候需要关心它：画面黑屏、Shader 报错、几何体不显示、深度关系错误时。

#### Mesa

- 它是什么：Linux 上常见的开源 OpenGL/Vulkan 图形驱动实现集合。
- 简短历史：Mesa 由 Brian Paul 在 1993 年开始开发，后来成为 Linux 图形栈的重要部分。
- 本项目中负责什么：在 Ubuntu/WSL2 环境里提供 OpenGL 运行能力和检测工具。
- 什么时候需要关心它：`glxinfo -B` 报错、OpenGL 版本过低、渲染环境不可用时。

#### GLU

- 它是什么：OpenGL Utility Library，早期 OpenGL 的辅助工具库。
- 简短历史：GLU 属于传统 OpenGL 生态，现代项目用得较少，但系统依赖里仍常见。
- 本项目中负责什么：主要作为 OpenGL 开发环境的一部分安装，第一版引擎不依赖它的高级接口。
- 什么时候需要关心它：系统包缺失导致 OpenGL 示例或老教程代码编译失败时。

#### mesa-utils / glxinfo / glxgears

- 它是什么：Mesa 提供的一组图形环境检查工具。
- 简短历史：这些工具长期用于 Linux 桌面和驱动排查。
- 本项目中负责什么：`glxinfo -B` 查看 OpenGL 信息，`glxgears` 验证窗口和基本渲染能跑。
- 什么时候需要关心它：确认 WSLg 是否能显示窗口、OpenGL 是否可用时。

#### glad

- 它是什么：OpenGL 函数加载器，用来获取当前环境支持的 OpenGL 函数地址。
- 简短历史：现代 OpenGL 函数通常需要运行时加载，glad 是常用的生成式加载器之一。
- 本项目中负责什么：在 SDL2 创建 OpenGL 上下文后，加载 OpenGL API。
- 什么时候需要关心它：OpenGL 函数指针为空、调用现代 OpenGL 函数崩溃时。

### 2.4 引擎辅助库

#### GLM

- 它是什么：面向 OpenGL 风格的 C++ 数学库，提供向量、矩阵和变换函数。
- 简短历史：GLM 的接口设计接近 GLSL，方便在 CPU 端写出和 Shader 相似的数学代码。
- 本项目中负责什么：计算 `Model`、`View`、`Projection` 矩阵，处理相机和 Transform。
- 什么时候需要关心它：物体位置不对、相机方向不对、矩阵乘法顺序混乱时。

#### stb_image

- 它是什么：单头文件图片加载库，可以读取 PNG、JPG 等常见格式。
- 简短历史：stb 系列由 Sean Barrett 推出，以“一个头文件解决一个问题”的方式流行起来。
- 本项目中负责什么：读取 `Assets/Textures` 里的贴图数据，再上传给 OpenGL。
- 什么时候需要关心它：贴图加载失败、图片通道数不对、纹理上下颠倒时。

#### Assimp

- 它是什么：Open Asset Import Library，用来读取 FBX、OBJ、glTF 等模型格式。
- 简短历史：Assimp 是 C/C++ 生态中常用的模型导入库，目标是统一各种 3D 文件格式。
- 本项目中负责什么：v0.1 之后用于加载复杂模型，第一阶段先不急着接入。
- 什么时候需要关心它：从手写立方体升级到真实模型、材质和骨骼动画时。

## 3. 学习路线总览

这个项目按 8 个阶段推进：

```text
Linux/WSL2 编译环境
-> C++ 引擎必备基础
-> CMake 工程搭建
-> SDL2 窗口和输入
-> OpenGL 清屏、三角形、立方体
-> Core / Renderer / Scene / Resource 模块拆分
-> Runtime 生命周期维护
-> Sandbox 验证和 Tests
```

每个阶段都按这个节奏学习：

```text
理解目标
-> 写最小代码
-> 编译
-> 运行
-> 观察结果
-> 修正常见错误
-> 进入下一阶段
```

第一条铁律：先让程序跑起来。
第二条铁律：每次只解决一个核心问题。
第三条铁律：资源谁创建，谁负责释放。

## 4. 项目目录设计

建议目录：

```text
Cpp_Engine_by_hand/
  CMakeLists.txt
  README.md
  Engine/
    CMakeLists.txt
    Core/
    Platform/
    Renderer/
    Scene/
    Resource/
  Sandbox/
    CMakeLists.txt
    main.cpp
  Assets/
    Shaders/
    Textures/
    Models/
  Tests/
    CMakeLists.txt
  third_party/
```

模块职责：

- `Engine/Core`：基础系统，负责 `Application`、主循环、日志、断言、时间。
- `Engine/Platform`：平台层，第一版只封装 SDL2 的窗口和输入。
- `Engine/Renderer`：渲染层，封装 OpenGL、Shader、Buffer、Texture、Camera、Mesh。
- `Engine/Scene`：场景层，管理 Entity、Component、Transform、Camera、MeshRenderer。
- `Engine/Resource`：资源层，管理 Shader、Texture、Mesh 的加载、缓存和释放。
- `Sandbox`：示例程序，用来验证引擎是否好用。
- `Assets`：放 Shader、贴图和模型。
- `Tests`：放单元测试和 smoke test。

依赖方向固定为：

```text
Sandbox -> Engine
Scene -> Renderer / Resource / Core
Renderer -> Platform / Core
Resource -> Core
Platform -> Core
```

不要让依赖反过来：

- `Renderer` 不要依赖 `Scene`。
- `Platform` 不要知道游戏对象。
- `Resource` 不要知道具体游戏玩法。
- `Sandbox` 可以调用引擎，但引擎不能调用 `Sandbox`。

## 5. 阶段一：Linux/WSL2 编译环境入门

本阶段目标：

让你知道如何在 Ubuntu 24.04 里编译和运行一个 C++ 程序。

你会学到：

- 终端里的 `cd`、`ls`、`mkdir`。
- `g++` 是什么。
- 源文件如何变成可执行程序。
- 编译错误和运行错误的区别。

建议练习：

创建 `hello.cpp`：

```cpp
#include <iostream>

int main()
{
    std::cout << "Hello Engine" << std::endl;
    return 0;
}
```

编译运行：

```bash
g++ hello.cpp -o hello
./hello
```

验收标准：

```text
Hello Engine
```

常见坑：

- `g++: command not found`：没有安装 `build-essential`。
- `./hello: No such file or directory`：没有在当前目录运行，或者编译失败。
- 修改代码后要重新编译。

## 6. 阶段二：C++ 引擎必备基础

本阶段目标：

不追求完整学完 C++，只先掌握写引擎必须用到的部分。

必须掌握：

- 头文件 `.h` 和源文件 `.cpp` 如何分工。
- `class` 和 `struct`。
- 构造函数和析构函数。
- 引用和指针。
- `std::string`。
- `std::vector`。
- `std::unique_ptr`。
- RAII。
- 资源所有权。

### 6.1 头文件和源文件

头文件声明接口：

```cpp
// Engine/Core/Application.h
#pragma once

class Application
{
public:
    void Run();
};
```

源文件实现逻辑：

```cpp
// Engine/Core/Application.cpp
#include "Application.h"
#include <iostream>

void Application::Run()
{
    std::cout << "Application running" << std::endl;
}
```

### 6.2 构造和析构

```cpp
class Window
{
public:
    Window();
    ~Window();
};
```

构造函数负责建立对象的初始状态。析构函数负责对象销毁时释放资源。

### 6.3 RAII 和资源所有权

RAII 的意思是：资源跟着对象的生命周期走。

比如窗口、纹理、Shader、Buffer 都是资源。创建了就要释放。C++ 里推荐让对象自己在析构时释放资源。

```cpp
class Texture
{
public:
    Texture(const char* path);
    ~Texture();

private:
    unsigned int m_RendererId = 0;
};
```

常见坑：

- 不要到处裸 `new` 和 `delete`。
- 能用 `std::unique_ptr` 表达唯一所有权时，就不要手动管理指针。
- 不要返回局部变量的引用或指针。
- 不清楚谁释放资源时，先停下来画生命周期。

验收标准：

- 能把一个类拆成 `.h` 和 `.cpp`。
- 能解释构造函数和析构函数什么时候执行。
- 能用 `std::vector` 存一组对象。
- 能用 `std::unique_ptr` 表示一个对象只属于一个所有者。

## 7. 阶段三：CMake 工程搭建

本阶段目标：

把项目从单个 `hello.cpp` 变成真正的 C++ 工程。

要写的模块：

- 根目录 `CMakeLists.txt`
- `Engine/CMakeLists.txt`
- `Sandbox/CMakeLists.txt`
- `Sandbox/main.cpp`

第一版构建关系：

```text
Engine  -> 静态库
Sandbox -> 可执行程序，链接 Engine
```

推荐命令：

```bash
cmake -S . -B build -G Ninja
cmake --build build
./build/Sandbox/Sandbox
```

第一阶段可运行成果：

```text
Hello Engine
```

验收标准：

- `cmake -S . -B build -G Ninja` 成功。
- `cmake --build build` 成功。
- `./build/Sandbox/Sandbox` 能运行。
- `Sandbox` 链接 `Engine`，不是复制引擎源码。

常见坑：

- 改了 CMake 后建议重新运行 `cmake -S . -B build -G Ninja`。
- include 路径要从目标配置，不要依赖编辑器猜。
- 不要把 build 目录提交到 Git。

## 8. 阶段四：Core 主循环

本阶段目标：

写出引擎最小生命体：`Application`。

要写的模块：

- `Application`
- `Layer`
- `TimeStep`
- `Log`
- `Assert`

核心接口方向：

```cpp
class Application
{
public:
    void Init();
    void Run();
    void Shutdown();

private:
    bool m_Running = true;
};
```

主循环先写成控制台版本：

```text
Init
-> while running
   -> Calculate delta time
   -> Update
   -> Render
-> Shutdown
```

验收标准：

- `Application::Run()` 能进入循环。
- 能打印每帧 `deltaTime`。
- 能通过一个退出条件结束循环。
- `Init()` 和 `Shutdown()` 的输出顺序清楚。

常见坑：

- 不要在构造函数里做大量初始化。
- 不要让全局变量控制整个程序。
- `deltaTime` 统一使用秒。

## 9. 阶段五：SDL2 窗口和输入

本阶段目标：

用 SDL2 打开窗口，并把窗口和输入封装进 `Platform` 层。

先确认 SDL2 能编译：

```bash
sdl2-config --cflags --libs
```

可以写一个临时 SDL2 窗口示例来验证环境：

```cpp
#include <SDL2/SDL.h>

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Probe",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_SHOWN
    );

    SDL_Delay(1000);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
```

编译：

```bash
g++ probe_sdl.cpp -o probe_sdl $(sdl2-config --cflags --libs)
./probe_sdl
```

要写的模块：

- `Window`
- `SDLWindow`
- `Input`
- `Event`

核心接口方向：

```cpp
struct WindowDesc
{
    int Width = 1280;
    int Height = 720;
    const char* Title = "Cpp Engine";
};
```

```cpp
class Window
{
public:
    virtual ~Window() = default;
    virtual void PollEvents() = 0;
    virtual void SwapBuffers() = 0;
    virtual bool ShouldClose() const = 0;
};
```

第二阶段可运行成果：

- SDL2 窗口能打开。
- 点击关闭按钮后程序能退出。
- 能读取键盘按键。
- 能读取鼠标位置。

常见坑：

- `SDL_Init` 和 `SDL_Quit` 要配对。
- 窗口销毁要在 SDL 退出之前。
- 输入要区分当前按下、本帧按下、本帧释放。
- 不要让 `Scene` 直接调用 SDL2 API。

## 10. 阶段六：OpenGL 清屏、三角形、立方体

本阶段目标：

从最小 OpenGL 程序开始，逐步理解渲染管线。

学习顺序：

```text
创建 OpenGL Context
-> 加载 OpenGL 函数
-> 设置清屏颜色
-> 编译 Shader
-> 创建 VertexBuffer
-> 绘制三角形
-> 加入 IndexBuffer
-> 绘制立方体
-> 加入深度测试
-> 加入相机矩阵
```

要写的模块：

- `GraphicsContext`
- `Renderer`
- `Shader`
- `VertexBuffer`
- `IndexBuffer`
- `VertexArray`
- `Texture2D`
- `Camera`
- `Mesh`

核心接口方向：

```cpp
class Renderer
{
public:
    void Init();
    void BeginFrame();
    void DrawMesh(const Mesh& mesh, const Shader& shader);
    void EndFrame();
    void Shutdown();
};
```

第三阶段可运行成果：

- 能创建 OpenGL 上下文。
- 能用指定颜色清屏。
- 能画一个三角形。
- 能画一个旋转立方体。
- 能开启深度测试，遮挡关系正确。
- 能用 WASD 和鼠标移动相机。

常见坑：

- OpenGL 函数必须在上下文创建后加载。
- Shader 编译失败要打印错误日志。
- 矩阵顺序固定为 `Projection * View * Model`。
- GPU 资源要在有效 OpenGL 上下文中释放。

## 11. 阶段七：模块拆分

本阶段目标：

把 Demo 代码拆成真正的引擎模块，避免所有逻辑堆在 `main.cpp`。

### 11.1 Core

负责：

- `Application`
- `Layer`
- `TimeStep`
- `Log`
- `Assert`

验收标准：

- `Application` 管主循环。
- `Layer` 可以挂载不同功能模块。
- 日志能输出初始化、更新、关闭信息。

### 11.2 Platform

负责：

- `Window`
- `SDLWindow`
- `Input`
- `Event`

验收标准：

- 上层通过 `Window` 接口使用窗口。
- SDL2 细节留在 `Platform` 层。

### 11.3 Renderer

负责：

- `Renderer`
- `Shader`
- `Buffer`
- `Texture`
- `Camera`
- `Mesh`

验收标准：

- `Renderer` 可以清屏和提交 Mesh。
- Shader、Buffer、Texture 都有清楚的创建和释放逻辑。

### 11.4 Scene

负责：

- `Scene`
- `Entity`
- `TransformComponent`
- `CameraComponent`
- `MeshRendererComponent`

基础组件：

```cpp
struct TransformComponent
{
    glm::vec3 Position {0.0f};
    glm::vec3 Rotation {0.0f};
    glm::vec3 Scale {1.0f};
};
```

```cpp
struct MeshRendererComponent
{
    MeshHandle Mesh;
    MaterialHandle Material;
};
```

验收标准：

- 能创建和销毁实体。
- 每个实体可以有 Transform。
- 场景里能放多个对象。
- 相机可以作为实体存在。
- `Renderer` 不主动查找 Scene，只接收绘制数据。

### 11.5 Resource

负责：

- `AssetManager`
- `TextureLoader`
- `ShaderLoader`
- `MeshLoader`
- 资源句柄类型

核心接口方向：

```cpp
class AssetManager
{
public:
    TextureHandle LoadTexture(const std::string& path);
    ShaderHandle LoadShader(const std::string& vertexPath, const std::string& fragmentPath);
    MeshHandle LoadMesh(const std::string& path);
    void UnloadUnused();
    void Shutdown();
};
```

验收标准：

- 重复加载同一路径资源时命中缓存。
- Shader 从 `Assets/Shaders` 读取。
- Texture 从 `Assets/Textures` 读取。
- 引擎关闭时统一释放资源。

常见坑：

- 模块拆分不是为了文件变多，而是为了职责清楚。
- 不要让 `Renderer` 依赖 `Scene`。
- 不要让资源加载器依赖具体游戏玩法。
- 资源句柄比裸指针更容易管理生命周期。

## 12. 阶段八：Runtime 生命周期维护

本阶段目标：

把所有系统串成稳定运行时，明确每个系统何时创建、更新、渲染和销毁。

启动流程：

```text
Application::Init
-> Log::Init
-> Window::Create
-> GraphicsContext::Init
-> Renderer::Init
-> AssetManager::Init
-> Scene::Load
```

每帧流程：

```text
Window::PollEvents
-> Input::Update
-> Scene::Update(deltaTime)
-> Renderer::BeginFrame
-> Scene::Render
-> Renderer::EndFrame
-> Window::SwapBuffers
```

关闭流程：

```text
Scene::Unload
-> AssetManager::Shutdown
-> Renderer::Shutdown
-> GraphicsContext::Shutdown
-> Window::Destroy
-> Log::Shutdown
```

验收标准：

- 空场景能正常运行和关闭。
- 加载失败时能打印错误并优雅退出。
- 关闭时不崩溃。
- 不重复释放 GPU 资源。
- 每个模块的 `Init()` 和 `Shutdown()` 顺序能在日志里看到。

常见坑：

- 初始化顺序和销毁顺序要相反。
- 不要在 `Shutdown()` 后访问已释放资源。
- 先保证稳定，再做复杂功能。

## 13. 阶段九：Sandbox 验证和 Tests

本阶段目标：

用 `Sandbox` 验证引擎能力，用测试保护核心逻辑。

Sandbox 第一个目标：

- 打开窗口。
- 清屏。
- 显示一个可旋转彩色立方体。
- 可以用键盘和鼠标移动相机。

Sandbox 第二个目标：

- 加载贴图。
- 创建简单 Scene。
- 支持多个对象和 Transform。

Sandbox 第三个目标：

- 整理资源生命周期。
- 补上测试。
- 写清楚 v0.1 使用方式。

推荐测试：

- 数学函数和矩阵计算。
- `AssetManager` 重复加载缓存。
- 模块初始化和关闭顺序。
- 空场景运行稳定性。
- 渲染 smoke test：创建窗口、清屏、跑几帧后关闭。

测试命令：

```bash
cmake -S . -B build -G Ninja
cmake --build build
ctest --test-dir build
```

常见坑：

- 渲染测试先做 smoke test，不要一开始追求精确像素。
- 测试不要要求用户手动点击窗口。
- `Sandbox` 只写示例玩法，不要把底层引擎功能写死在里面。

## 14. v0.1 完成标准

当下面这些都完成时，项目进入 v0.1：

- Ubuntu 24.04 下能通过 CMake + Ninja 构建。
- `Sandbox` 可以运行一个 3D 场景。
- 场景里有多个实体。
- 每个实体有 Transform。
- 至少一个对象使用贴图。
- 相机可移动。
- 引擎启动和关闭顺序清楚。
- 重复加载资源会命中缓存。
- 空场景不会崩溃。
- README 中的构建命令可以跑通。

## 15. 常用命令速查

这里的命令负责安装、检查、构建、运行和测试；每个工具的作用和背景见第 2 节。

安装依赖：

```bash
sudo apt update
sudo apt install -y build-essential cmake ninja-build gdb git pkg-config
sudo apt install -y libsdl2-dev libgl1-mesa-dev libglu1-mesa-dev mesa-utils
```

检查图形环境：

```bash
echo $DISPLAY
glxinfo -B
glxgears
```

配置工程：

```bash
cmake -S . -B build -G Ninja
```

构建工程：

```bash
cmake --build build
```

运行 Sandbox：

```bash
./build/Sandbox/Sandbox
```

运行测试：

```bash
ctest --test-dir build
```

清理构建目录：

```bash
rm -rf build
```

## 16. 后续扩展方向

v0.1 之后可以继续做：

- Assimp 模型加载。
- 材质系统。
- 光照和阴影。
- ImGui 编辑器面板。
- 场景保存和加载。
- 简单物理。
- 音频系统。
- 脚本系统。
- 多平台构建。

推荐顺序：

```text
模型加载
-> 材质系统
-> ImGui Inspector
-> 场景保存和加载
-> 光照
-> 物理
```

## 17. 一句话路线

先在 WSL2 Ubuntu 24.04 里跑通 C++、CMake、SDL2 和 OpenGL，再从 `Hello Engine` 一步步推进到窗口、清屏、三角形、立方体、相机、Scene、AssetManager，最后把所有模块的生命周期整理成一个稳定的 3D 引擎 v0.1。
