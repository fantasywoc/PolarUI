#include "UIWindow.h"
#include "component/UIPanel.h"
#include "component/UIButton.h"
#include "component/UILabel.h"
#include "component/FlexLayout.h"
#include <iostream>
#include <memory>
#include "component/UITexture.h"
#include "component/UITextInput.h"
// 添加动画系统头文件
#include "animation/UIAnimationManager.h"
#include "animation/UIAnimation.h"

void printComponentInfo(const std::string& name, std::shared_ptr<UIComponent> component) {
    std::cout << name << ": Position(" << component->getX() << ", " << component->getY() 
              << "), Size(" << component->getWidth() << " x " << component->getHeight() << ")" << std::endl;
}

void printAllComponentsInfo(std::shared_ptr<UIPanel> mainPanel, 
                           std::shared_ptr<UIPanel> leftPanel, 
                           std::shared_ptr<UIPanel> rightPanel,
                           std::shared_ptr<UILabel> label,
                           std::shared_ptr<UIButton> button1,
                           std::shared_ptr<UIButton> button2,
                           std::shared_ptr<UIButton> button3,
                           std::shared_ptr<UIButton> okButton,
                           std::shared_ptr<UIButton> exitButton) {
    std::cout << "\n=== Component Positions Before Rendering ===" << std::endl;
    printComponentInfo("Main Panel", mainPanel);
    printComponentInfo("Left Panel", leftPanel);
    printComponentInfo("Right Panel", rightPanel);
    std::cout << "--- Left Panel Children ---" << std::endl;
    printComponentInfo("Label", label);
    printComponentInfo("Button 1", button1);
    printComponentInfo("Button 2", button2);
    printComponentInfo("Button 3", button3);
    std::cout << "--- Right Panel Children ---" << std::endl;
    printComponentInfo("OK Button", okButton);
    printComponentInfo("Exit Button", exitButton);
    std::cout << "==========================================\n" << std::endl;
}

int main() {
    UIWindow window(1300, 900, "Button Demo with Animations");
    
    // 设置透明帧缓冲区
    window.setTransparentFramebuffer(true);
    
    // 初始化窗口
    if (!window.initialize()) {
        std::cerr << "Failed to initialize window" << std::endl;
        return -1;
    }
    
    // 创建主面板
    auto mainPanel = std::make_shared<UIPanel>(10, 10, 1200, 800);
    mainPanel->setHorizontalLayoutWithAlignment(FlexLayout::X_CENTER, FlexLayout::Y_CENTER, 0.0f, 0.0f);
    mainPanel->setBackgroundColor(nvgRGBA(100, 200, 100, 10));
    mainPanel->setBorderColor(nvgRGBA(255, 255, 255,200));
    mainPanel->setBorderWidth(1.0f);
    mainPanel->setCornerRadius(10.0f);

    // 创建左面板 - 使用具体坐标（由布局系统自动计算）
    auto leftPanel = std::make_shared<UIPanel>(0, 0, 320, 480);
    leftPanel->setVerticalLayoutWithAlignment(FlexLayout::X_CENTER, FlexLayout::Y_START, 10.0f, 10.0f);
    leftPanel->setBackgroundColor(nvgRGB(100, 150, 255));
    
    // 创建右面板
    auto rightPanel = std::make_shared<UIPanel>(0, 0, 320, 480);
    rightPanel->setHorizontalLayoutWithAlignment(FlexLayout::X_CENTER, FlexLayout::Y_CENTER, 10.0f, 10.0f);
    rightPanel->setBackgroundColor(nvgRGB(255, 100, 100));
    
    // 创建右图像面板
    auto rightPanel1 = std::make_shared<UIPanel>(0, 0, 500, 700);
    rightPanel1->setVerticalLayoutWithAlignment(FlexLayout::X_CENTER, FlexLayout::Y_CENTER, 10.0f, 10.0f);
    rightPanel1->setBackgroundColor(nvgRGBA(255, 100, 100, 100));


    // 添加标签到左面板 - 修改宽度为150px
    auto label = std::make_shared<UILabel>(0, 0, 150, 30, "Left Panel Label");
    label->setTextAlign(UILabel::TextAlign::CENTER);

    
    // 创建按钮 - 统一宽度为150px，并添加动画效果
    auto button1 = std::make_shared<UIButton>(0, 0, 150, 40, "Fade Animation");
    button1->setCornerRadius(10.0f);
    button1->setOnClick([button1]() {
        std::cout << "Button 1 clicked! Playing fade animation..." << std::endl;
        // 淡出再淡入的动画效果
        UIAnimationManager::getInstance().fadeOut(button1.get(), 0.3f, UIAnimation::EASE_IN);
        
        // 创建延迟淡入动画
        auto fadeInAnim = std::make_shared<UIAnimation>(UIAnimation::FADE, 0.3f, UIAnimation::EASE_OUT);
        fadeInAnim->setValues(0.0f, 1.0f);
        fadeInAnim->setOnUpdate([button1](float value) {
            button1->setAnimationOpacity(value);
        });
        
        // 延迟0.1秒后开始淡入
        auto delayAnim = std::make_shared<UIAnimation>(UIAnimation::CUSTOM, 0.1f);
        delayAnim->setOnComplete([fadeInAnim, button1]() {
            UIAnimationManager::getInstance().addAnimation(fadeInAnim, button1.get());
        });
        UIAnimationManager::getInstance().addAnimation(delayAnim, button1.get());
    });
    
    auto button2 = std::make_shared<UIButton>(0, 0, 150, 40, "Scale Animation");
    button2->setOnClick([button2]() {
        std::cout << "Button 2 clicked! Playing scale animation..." << std::endl;
        
        // 先移除该组件的所有动画，避免冲突
        UIAnimationManager::getInstance().removeAnimation(button2.get());
        
        // 优化后的缩放动画：使用中心坐标作为缩放原点
        UIAnimationManager::getInstance().scaleTo(button2.get(), 1.05f, 1.05f, 0.1f, UIAnimation::EASE_OUT, UIAnimation::CENTER);
        
        // 创建恢复缩放的动画 - 使用 EASE_IN_OUT 实现平滑过渡
        auto scaleBackAnim = std::make_shared<UIAnimation>(UIAnimation::SCALE, 0.2f, UIAnimation::EASE_IN_OUT);
        scaleBackAnim->setScaleOrigin(UIAnimation::CENTER); // 设置缩放原点为中心
        scaleBackAnim->setValues(1.05f, 1.0f);
        scaleBackAnim->setOnUpdate([button2](float value) {
            button2->setAnimationScaleX(value);
            button2->setAnimationScaleY(value);
        });
        
        // 缩短延迟时间，让动画更连贯
        auto delayAnim = std::make_shared<UIAnimation>(UIAnimation::CUSTOM, 0.05f);
        delayAnim->setOnComplete([scaleBackAnim, button2]() {
            UIAnimationManager::getInstance().addAnimation(scaleBackAnim, button2.get());
        });
        UIAnimationManager::getInstance().addAnimation(delayAnim, button2.get());
    });
    
    auto button3 = std::make_shared<UIButton>(0, 0, 150, 150, "Rotate Animation");
    button3->setCornerRadius(75.0f);
    button3->setOnClick([button3]() {
        std::cout << "Button 3 clicked! Playing rotation animation..." << std::endl;
        
        // 先移除该组件的所有动画，避免冲突
        UIAnimationManager::getInstance().removeAnimation(button3.get());
        
        // 旋转动画：360度旋转
        UIAnimationManager::getInstance().rotateTo(button3.get(), 360.0f, 1.0f, UIAnimation::EASE_IN_OUT);
        
        // 使用更简单的方式处理重置
        auto resetAnim = std::make_shared<UIAnimation>(UIAnimation::CUSTOM, 1.0f);
        resetAnim->setOnComplete([button3]() {
            // 重置旋转角度
            button3->setAnimationRotation(0.0f);
        });
        UIAnimationManager::getInstance().addAnimation(resetAnim, button3.get());
    });
    
    leftPanel->addChild(button1);
    leftPanel->addChild(button2);
    leftPanel->addChild(button3);
    
    // 添加输入框到左面板
    auto inputBox = std::make_shared<UITextInput>(0, 0, 200, 30, "Please input text...");
    inputBox->setTextColor(nvgRGB(255, 255, 255));
    inputBox->setBackgroundColor(nvgRGB(50, 50, 50));
    inputBox->setBorderColor(nvgRGB(100, 100, 100));
    inputBox->setFocusedBorderColor(nvgRGB(100, 150, 200));
    inputBox->setCornerRadius(4.0f);
    inputBox->setBorderWidth(1.0f);
    
    // 设置输入框的回调函数，添加焦点动画
    inputBox->setOnTextChanged([](const std::string& text) {
        std::cout << "Text changed: " << text << std::endl;
    });
    
    inputBox->setOnEnterPressed([](const std::string& text) {
        std::cout << "Enter pressed with text: " << text << std::endl;
    });
    
    inputBox->setOnFocusChanged([inputBox](bool focused) {
        std::cout << "Focus changed: " << (focused ? "focused" : "unfocused") << std::endl;
        if (focused) {
            // 获得焦点时轻微放大
            UIAnimationManager::getInstance().scaleTo(inputBox.get(), 1.55f, 1.5f, 0.2f, UIAnimation::EASE_OUT);
        } else {
            // 失去焦点时恢复原大小
            UIAnimationManager::getInstance().scaleTo(inputBox.get(), 1.0f, 1.0f, 0.2f, UIAnimation::EASE_IN);
        }
    });
    
    leftPanel->addChild(inputBox);
    
    // 先创建纹理组件
    auto texture = std::make_shared<UITexture>(0, 0, 400, 600, "D:\\Picture\\JEPG\\20250216\\20250216-P1013191-.jpg");
    texture->setScaleMode(UITexture::ScaleMode::KEEP_ASPECT);
    texture->setAlpha(0.9f);
    texture->setCornerRadius(10.0f);
    texture->setBorderColor(nvgRGBA(255, 255, 255, 100));
    texture->setBorderWidth(2.0f);
    
    // 右面板的按钮，添加动画效果
    auto okButton = std::make_shared<UIButton>(0, 0, 70, 40, "OK");
    okButton->setOnClick([okButton, mainPanel,rightPanel1]() {
        std::cout << "OK button clicked!" << std::endl;
        
        // 检查图片当前的可见状态
        bool isVisible = rightPanel1->isDisplay();
        
        if (isVisible) {
            // 当前可见，执行隐藏动画
            rightPanel1->setDisplay(false);
            okButton->setText("Hide");
            mainPanel->updateLayout();
            std::cout << "Hiding texture..." << std::endl;
        } else {
            // 当前隐藏，执行显示动画
            rightPanel1->setDisplay(true);
            okButton->setText("show");
            mainPanel->updateLayout();
            std::cout << "Showing texture..." << std::endl;
        }
        
        // 成功动画：绿色闪烁效果
        auto flashAnim = std::make_shared<UIAnimation>(UIAnimation::CUSTOM, 0.5f, UIAnimation::EASE_IN_OUT);
        flashAnim->setValues(0.0f, 1.0f);
        flashAnim->setRepeatCount(2); // 重复2次
        flashAnim->setReverse(true);  // 反向播放
        flashAnim->setOnUpdate([okButton](float value) {
            // 在原色和绿色之间插值
            NVGcolor originalColor = nvgRGB(100, 100, 100);
            NVGcolor flashColor = nvgRGB(100, 255, 100);
            NVGcolor currentColor = nvgLerpRGBA(originalColor, flashColor, value);
            okButton->setBackgroundColor(currentColor);
        });
        UIAnimationManager::getInstance().addAnimation(flashAnim, okButton.get());
    });
    
    auto exitButton = std::make_shared<UIButton>(0, 0, 120, 40, "Exit");
    exitButton->setOnClick([&window, exitButton]() {
        std::cout << "Exit button clicked!" << std::endl;
        // 退出动画：红色闪烁后关闭
        auto flashAnim = std::make_shared<UIAnimation>(UIAnimation::CUSTOM, 0.3f, UIAnimation::EASE_IN_OUT);
        flashAnim->setValues(0.0f, 1.0f);
        flashAnim->setRepeatCount(3);
        flashAnim->setReverse(true);
        flashAnim->setOnUpdate([exitButton](float value) {
            NVGcolor originalColor = nvgRGB(100, 100, 100);
            NVGcolor flashColor = nvgRGB(255, 100, 100);
            NVGcolor currentColor = nvgLerpRGBA(originalColor, flashColor, value);
            exitButton->setBackgroundColor(currentColor);
        });
        flashAnim->setOnComplete([&window]() {
            glfwSetWindowShouldClose(window.getGLFWWindow(), GLFW_TRUE);
        });
        UIAnimationManager::getInstance().addAnimation(flashAnim, exitButton.get());
    });
    


    rightPanel->addChild(okButton);
    rightPanel->addChild(exitButton);
    rightPanel1->addChild(texture);
    rightPanel1->addChild(label);
    // 添加子面板到主面板
    mainPanel->addChild(leftPanel);
    mainPanel->addChild(rightPanel);
    mainPanel->addChild(rightPanel1);
    
    // 启动时的入场动画
    std::cout << "Starting entrance animations..." << std::endl;
    
//     // 主面板淡入
//     mainPanel->setAnimationOpacity(0.0f);
//     UIAnimationManager::getInstance().fadeIn(mainPanel.get(), 1.0f, UIAnimation::EASE_OUT);
    
//     // 左面板从左侧滑入 - 修正值计算
//     leftPanel->setAnimationOffsetX(-leftPanel->getWidth()); // 设置初始偏移
//     auto leftSlideAnim = std::make_shared<UIAnimation>(UIAnimation::MOVE, 3.0f, UIAnimation::EASE_OUT);
//     leftSlideAnim->setValues(0.0f, 1.0f); // 动画进度从0到1
//     leftSlideAnim->setOnUpdate([leftPanel](float progress) {
//     // 手动计算实际偏移量：从-width到0
//     float actualOffset = -leftPanel->getWidth() * (1.0f - progress);
//     leftPanel->setAnimationOffsetX(actualOffset);
//     std::cout << "Left panel offset: " << actualOffset << std::endl;
//     });
//     UIAnimationManager::getInstance().addAnimation(leftSlideAnim, leftPanel.get());
//     leftSlideAnim->start();  // 添加这行！
    
//    // 右面板从右侧滑入 - 使用 moveTo 方法
//     float rightTargetX = rightPanel->getX();  // 目标位置
//     rightPanel->setPosition(rightTargetX + rightPanel->getWidth(), rightPanel->getY());  // 设置初始位置在屏幕外
//     UIAnimationManager::getInstance().moveTo(rightPanel.get(), rightTargetX, rightPanel->getY(), 3.0f, UIAnimation::EASE_OUT);
        
//     // 图片面板弹跳进入
//     rightPanel1->setAnimationScaleX(0.0f);
//     rightPanel1->setAnimationScaleY(0.0f);
//     auto bounceAnim = std::make_shared<UIAnimation>(UIAnimation::SCALE, 1.2f, UIAnimation::BOUNCE);
//     bounceAnim->setValues(0.0f, 1.0f);
//     bounceAnim->setOnUpdate([rightPanel1](float value) {
//         rightPanel1->setAnimationScaleX(value);
//         rightPanel1->setAnimationScaleY(value);
//     });
//     UIAnimationManager::getInstance().addAnimation(bounceAnim, rightPanel1.get());
//     bounceAnim->start();  // 添加这行
    
    // 设置鼠标事件回调
    window.setMouseButtonCallback([mainPanel](int button, int action, int mods) {
        if (button == 0) { // 左键
            double xpos, ypos;
            glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);
            
            UIEvent event;
            event.type = (action == 1) ? UIEvent::MOUSE_PRESS : UIEvent::MOUSE_RELEASE;
            event.mouseX = static_cast<float>(xpos);
            event.mouseY = static_cast<float>(ypos);
            event.mouseButton = button;
            
            mainPanel->handleEvent(event);
        }
    });
    
    window.setCursorPosCallback([mainPanel](double xpos, double ypos) {
        UIEvent event;
        event.type = UIEvent::MOUSE_MOVE;
        event.mouseX = static_cast<float>(xpos);
        event.mouseY = static_cast<float>(ypos);
        event.mouseButton = -1;
        
        mainPanel->handleEvent(event);
    });
    
    // 在设置鼠标事件回调后添加键盘事件处理
    window.setKeyCallback([mainPanel](int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            UIEvent event;
            event.type = UIEvent::KEY_PRESS;
            event.keyCode = key;
            event.modifiers = mods;
            
            mainPanel->handleEvent(event);
        }
    });

    // 添加字符输入事件处理
    window.setCharCallback([mainPanel](unsigned int codepoint) {
        UIEvent event;
        event.type = UIEvent::CHAR_INPUT;
        event.character = codepoint;
        
        mainPanel->handleEvent(event);
    });
    
    // 在渲染前输出所有组件的位置和尺寸信息
    printAllComponentsInfo(mainPanel, leftPanel, rightPanel, label, button1, button2, button3, okButton, exitButton);
    
    // 主渲染循环
    auto lastTime = glfwGetTime();
    while (!window.shouldClose()) {
        window.pollEvents();
        
        // 计算时间差
        auto currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        // 更新动画系统
        UIAnimationManager::getInstance().update(deltaTime);
        
        window.beginFrame();
        window.clearBackground(0.0f, 0.0f, 0.0f, 0.0f);
        
        // 渲染主面板（会递归渲染所有子组件）
        mainPanel->render(window.getNVGContext());
        
        window.endFrame();
        window.swapBuffers();
    }
    
    window.cleanup();
    return 0;
}