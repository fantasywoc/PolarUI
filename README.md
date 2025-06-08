





# UI Window Project

这是一个基于 GLFW、OpenGL 和 NanoVG 的 UI 窗口项目，提供了一个封装良好的 `UIWindow` 类来简化窗口和图形界面的开发。

## UIWindow 类的主要功能

### 1. 窗口管理
- 创建、初始化、清理GLFW窗口
- 窗口生命周期管理
- 自动资源清理

### 2. 渲染上下文
- 管理OpenGL和NanoVG上下文
- 自动初始化GLEW
- 提供统一的渲染环境

### 3. 事件处理
- 封装鼠标、键盘事件处理
- 简化输入状态查询
- 支持光标位置获取

### 4. 渲染循环
- 提供便捷的渲染开始/结束方法
- 自动处理帧缓冲区管理
- 简化渲染流程

### 5. 回调系统
- 支持自定义事件回调函数
- 键盘、鼠标、窗口大小变化回调
- 类型安全的回调接口

### 6. 窗口属性
- 透明度、大小等属性设置
- 透明帧缓冲区支持
- 灵活的窗口配置选项

## 项目结构

```
UI/
├── src/
│   ├── UIWindow.h      # UIWindow 类头文件
│   ├── UIWindow.cpp    # UIWindow 类实现
│   └── main.cpp        # 示例程序
├── xmake.lua           # 构建配置
└── README.md           # 项目说明
```

## 依赖库

- **GLFW 3.3.8**: 窗口管理和输入处理
- **GLEW 1.13.0**: OpenGL 扩展加载
- **NanoVG**: 2D 矢量图形渲染

## 构建方法

使用 xmake 构建项目：

```bash
xmake
```

运行程序：

```bash
xmake run
```

## 使用示例

```cpp
#include "UIWindow.h"

int main() {
    // 创建窗口
    UIWindow window(800, 600, "My UI App");
    
    // 初始化
    if (!window.initialize()) {
        return -1;
    }
    
    // 设置透明度
    window.setWindowOpacity(0.8f);
    
    // 主循环
    while (!window.shouldClose()) {
        window.clearBackground();
        window.beginFrame();
        
        // 在这里添加你的渲染代码
        
        window.endFrame();
        window.swapBuffers();
        window.pollEvents();
    }
    
    return 0;
}
```

## 特性

- ✅ 跨平台支持（Windows/Linux/macOS）
- ✅ 透明窗口支持
- ✅ 事件回调系统
- ✅ 简洁的 API 设计
- ✅ 自动资源管理
- ✅ NanoVG 集成

## 许可证

本项目采用开源许可证，具体请查看 LICENSE 文件。

## 性能优化

### 常见的CPU和GPU占用高的问题

#### 问题原因
1. **无限制的渲染循环**: 主循环以最大速度运行，没有帧率限制
2. **缺少垂直同步**: 未启用VSync，导致GPU过度渲染
3. **每帧清除所有缓冲区**: 不必要的缓冲区清除操作
4. **事件处理效率低**: 频繁的事件轮询和处理

#### 解决方案

##### 1. 启用垂直同步 (VSync)
在 `UIWindow.cpp` 的 `initialize()` 方法中添加：
```cpp
// 启用垂直同步，限制帧率到显示器刷新率
glfwSwapInterval(1);
```
 

##### 2. 添加帧率控制
在主循环中添加帧率限制：
```cpp
#include <thread>
#include <chrono>

// 目标帧率 (60 FPS)
const double targetFrameTime = 1.0 / 60.0;
double lastTime = glfwGetTime();

while (!window.shouldClose()) {
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - lastTime;
    
    if (deltaTime >= targetFrameTime) {
        // 渲染和更新逻辑
        window.clearBackground();
        window.beginFrame();
        // ... 渲染代码 ...
        window.endFrame();
        window.swapBuffers();
        
        lastTime = currentTime;
    } else {
        // 休眠剩余时间
        auto sleepTime = std::chrono::duration<double>(targetFrameTime - deltaTime);
        std::this_thread::sleep_for(sleepTime);
    }
    
    window.pollEvents();
}
```

##### 3. 优化缓冲区清除
在 `UIWindow.cpp` 的 `clearBackground()` 方法中：
```cpp
void UIWindow::clearBackground() {
    // 只清除颜色缓冲区，避免不必要的深度和模板缓冲区清除
    glClear(GL_COLOR_BUFFER_BIT);
}
```

##### 4. 事件处理优化
- 使用 `glfwWaitEvents()` 替代 `glfwPollEvents()` 在不需要连续渲染时
- 实现脏矩形更新，只在需要时重绘
- 合理使用事件回调，避免频繁的状态查询

### 性能监控

可以添加简单的性能监控来跟踪帧率：
```cpp
// 帧率计算
static int frameCount = 0;
static double lastFPSTime = glfwGetTime();

frameCount++;
double currentTime = glfwGetTime();
if (currentTime - lastFPSTime >= 1.0) {
    double fps = frameCount / (currentTime - lastFPSTime);
    std::cout << "FPS: " << fps << std::endl;
    frameCount = 0;
    lastFPSTime = currentTime;
}
```

### 推荐配置

对于大多数应用，推荐以下配置：
- 启用VSync (`glfwSwapInterval(1)`)
- 限制帧率到60 FPS
- 只在UI有变化时重绘
- 使用适当的事件处理策略

这些优化可以显著降低CPU和GPU的使用率，同时保持流畅的用户体验。

// ... existing code ...



        





          
# 控件布局
根据FlexLayout的实现，控件间距和边距是通过以下两个参数控制的：

## 控件间距和边距控制

### 1. **Spacing（间距）**
- **作用**：控制相邻子控件之间的距离
- **参数**：`spacing`（float类型）
- **应用**：在水平布局中控制左右相邻控件的间距，在垂直布局中控制上下相邻控件的间距

### 2. **Padding（内边距）**
- **作用**：控制第一个子控件与父控件边缘的距离，以及最后一个子控件与父控件边缘的距离
- **参数**：`padding`（float类型）
- **应用**：在所有方向上为容器内容提供内边距

## 当前代码中的使用示例

在 <mcfile name="main.cpp" path="e:\Program\Learning-progect\UI\src\main.cpp"></mcfile> 中：

```cpp
// 主面板：水平布局，间距20px，内边距15px
mainPanel->setHorizontalLayout(FlexLayout::START, 20.0f, 15.0f);

// 左面板：垂直布局，间距10px，内边距10px
leftPanel->setVerticalLayout(FlexLayout::START, 10.0f, 10.0f);
```

## 布局方法签名

```cpp
void setHorizontalLayout(FlexLayout::Alignment alignment, float spacing, float padding);
void setVerticalLayout(FlexLayout::Alignment alignment, float spacing, float padding);
```

## 参数详解

### Spacing（第二个参数）
- **水平布局**：控制左右相邻控件的水平间距
- **垂直布局**：控制上下相邻控件的垂直间距
- **计算方式**：`总间距 = (子控件数量 - 1) × spacing`

### Padding（第三个参数）
- **作用范围**：四个方向都应用相同的内边距
- **计算方式**：
  - 可用宽度 = `容器宽度 - 2 × padding`
  - 可用高度 = `容器高度 - 2 × padding`
  - 起始位置 = `容器位置 + padding`

## 实际效果示例

以左面板为例（垂直布局，spacing=10px，padding=10px）：
```
┌─────────────────────────────────┐
│ ↑ 10px padding                 │
│ ┌─────────────────────────────┐ │
│ │ Label                       │ │
│ └─────────────────────────────┘ │
│ ↕ 10px spacing                  │
│ ┌─────────────────────────────┐ │
│ │ Button 1                    │ │
│ └─────────────────────────────┘ │
│ ↕ 10px spacing                  │
│ ┌─────────────────────────────┐ │
│ │ Button 2                    │ │
│ └─────────────────────────────┘ │
│ ↕ 10px spacing                  │
│ ┌─────────────────────────────┐ │
│ │ Button 3                    │ │
│ └─────────────────────────────┘ │
│ ↓ 10px padding                 │
└─────────────────────────────────┘
```

## 如何调整

如果你想要修改间距和边距，可以调用相应的设置方法：

```cpp
// 增大控件间距到20px，内边距到15px
leftPanel->setVerticalLayout(FlexLayout::START, 20.0f, 15.0f);

// 或者单独设置
auto layout = leftPanel->getFlexLayout();
if (layout) {
    layout->setSpacing(20.0f);  // 设置间距
    layout->setPadding(15.0f);  // 设置内边距
    leftPanel->updateLayout();  // 更新布局
}
```
        