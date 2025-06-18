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
#include "TinyEXIF/EXIF.h"
#include <locale>
#include <windows.h>  // 添加这行
#include <io.h>       // 添加这行
#include <fcntl.h>    // 添加这行
#include <filesystem>

#include <vector>
#include <algorithm>
#include <set>

namespace fs = std::filesystem;

#include <filesystem>
#include <vector>
#include <set>
#include <algorithm>
#include <iostream>

namespace fs = std::filesystem;

// 查找指定目录下的图片文件路径和文件名
void find_image_files(
    const fs::path& directory, 
    std::vector<fs::path>& image_paths,
    std::vector<std::string>& image_names
) {
    // 清空结果容器确保每次调用都是全新结果
    image_paths.clear();
    image_names.clear();

    // 支持的图片扩展名（统一小写）[9](@ref)
    static const std::set<std::string> image_extensions = {
        ".jpg", ".jpeg", ".png", ".bmp", ".gif", ".tiff", ".webp"
    };

    // 验证目录有效性 [6,8](@ref)
    if (!fs::exists(directory)) {
        std::cerr << "错误：路径不存在 - " << directory << std::endl;
        return;
    }
    if (!fs::is_directory(directory)) {
        std::cerr << "错误：目标不是目录 - " << directory << std::endl;
        return;
    }

    try {
        // 创建递归迭代器（跳过权限错误）[7,8](@ref)
        auto options = fs::directory_options::skip_permission_denied;
        fs::recursive_directory_iterator dir_iter(directory, options);
        fs::recursive_directory_iterator end_iter;

        while (dir_iter != end_iter) {
            try {
                const auto& entry = *dir_iter;
                
                // 跳过非常规文件（目录/符号链接等）[9](@ref)
                if (entry.is_regular_file()) {
                    // 获取小写扩展名以统一比较
                    std::string ext = entry.path().extension().string();
                    std::transform(ext.begin(), ext.end(), ext.begin(), 
                                  [](unsigned char c) { return std::tolower(c); });
                    
                    // 检查是否为图片格式 [9](@ref)
                    if (image_extensions.find(ext) != image_extensions.end()) {
                        image_paths.push_back(entry.path());
                        image_names.push_back(entry.path().filename().string());
                    }
                }
                ++dir_iter;
            } 
            // 捕获单个文件处理中的异常（不影响整体遍历）
            catch (const fs::filesystem_error& e) {
                std::cerr << "跳过无法访问的文件: " << e.what() << std::endl;
                dir_iter.disable_recursion_pending();  // 继续遍历其他文件 [7](@ref)
                ++dir_iter;
            }
        }
    } 
    // 捕获全局性异常（如目录打开失败）
    catch (const fs::filesystem_error& e) {
        std::cerr << "文件系统错误: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "未知错误: " << e.what() << std::endl;
    }
}

 
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


// void printImagePath(){

//     std::vector<fs::path> image_paths;
//     image_paths = find_image_files("D:/Picture/Saved Pictures/");
//     if (image_paths.empty()){  // 修改：is_empty() -> empty()
//         std::cout<<"image_paths is empty!"<<std::endl;
//         return ;
//     }
    
//     for (const auto& path : image_paths) {
//         std::cout << "Image Path: " << path.string() << std::endl;
//     }
// }



int main() {

    // 设置控制台输出为UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // 在现有代码中添加
    UIWindow window(1600, 900, "Button Demo with Animations");
    
    
    // 设置透明帧缓冲区
    window.setTransparentFramebuffer(true);
    
    // 初始化窗口
    if (!window.initialize()) {
        std::cerr << "Failed to initialize window" << std::endl;
        return -1;
    }
    // 方法1: 使用现有的帧缓冲区方法
    int fbWidth, fbHeight ;
    window.getFramebufferSize(fbWidth, fbHeight);
    std::cout << "帧缓冲区大小: " << fbWidth << "x" << fbHeight << std::endl;
    
 


    int centerX = fbWidth / 2.0f;
    int centerY = fbHeight / 2.0f;
    int windowWidth{1600},windowHeight{900};
    // 创建主面板
    auto mainPanel = std::make_shared<UIPanel>(0, 0, windowWidth, windowHeight);
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
    auto rightPanel = std::make_shared<UIPanel>(0, 0, windowWidth, windowHeight);
    rightPanel->setVerticalLayoutWithAlignment(FlexLayout::X_CENTER, FlexLayout::Y_CENTER, 10.0f, 10.0f);
    rightPanel->setBackgroundColor(nvgRGBA(255, 255, 255,200));
    
    // 创建右图像面板
    auto rightPanel1 = std::make_shared<UIPanel>(0, 0,  windowWidth, windowHeight);
    rightPanel1->setVerticalLayoutWithAlignment(FlexLayout::X_CENTER, FlexLayout::Y_CENTER, 10.0f, 10.0f);
    rightPanel1->setBackgroundColor(nvgRGBA(255, 255, 255,200));

    // 获取图片文件
    const fs::path directory = "D:/Picture/Saved Pictures/";
    std::vector<fs::path> image_paths;
    std::vector<std::string> image_names;
    find_image_files(directory,image_paths,image_names);

    int current_index =15;
    size_t limit_index =image_paths.size(); 
    std::string imagPath =image_paths[current_index].generic_string();
    int change_speed = 0;
    EXIF exif(imagPath);
    exif.printAllInfo();


    // 先创建纹理组件
    // auto texture = std::make_shared<UITexture>(0, 0, 400, 600, "D:\\Picture\\JEPG\\20250216\\20250216-P1013191-.jpg");
    auto texture = std::make_shared<UITexture>(0, 0, windowHeight*0.7f, windowHeight*0.7, imagPath);

    texture->setScaleMode(UITexture::ScaleMode::KEEP_ASPECT);
    texture->setAlpha(1.0f);
    texture->setCornerRadius(10.0f);
    // texture->setBorderColor(nvgRGBA(255, 255, 255, 100));
    // texture->setBorderWidth(2.0f);



// 方法2: 设置窗口大小变化监听
   window.setWindowSizeCallback([&,mainPanel,rightPanel1](int width, int height) {
    std::cout << "窗口大小实时更新: " << width << "x" << height << std::endl;
    // int centerX = (width - mainPanel->getWidth()/2.0f);
    // int centerY = (height - mainPanel->getHeight()/2.0f);
    // mainPanel->setPosition(centerX, centerY);
    mainPanel->setSize(width,height);
    rightPanel1->setSize(width,height);
    
    texture->setSize(width*0.8,height*0.8);
    texture->setOriginWidth(height*0.7);
    
});



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
    

  
  
    float scalex{1}, scaley{1};
    // 右面板的按钮，添加动画效果
    auto okButton = std::make_shared<UIButton>(0, 0, 120, 50, "SCALE OUT");
    okButton->setOnClick([&,texture,label, okButton, rightPanel1, mainPanel, rightPanel]() {
        std::cout << "\n=== OK按钮点击事件开始 ===" << std::endl;
        


        if (scalex <= 10.0f) {
            scalex += 0.2f;
            scaley += 0.2f;  
        }
        else{
            scalex = 10.0f;
            scaley = 10.0f; 
        }

        UIAnimationManager::getInstance().scaleTo(texture.get(), 1.0f * scalex, 1.0f *scaley, 0.35f, UIAnimation::EASE_OUT);


        // === 执行布局更新 ===
        std::cout << "\n--- 开始布局更新 ---" << std::endl;
        mainPanel->updateLayout();
        
        // 使用递归方法重置所有组件的动画偏移
        // mainPanel->resetAllAnimationOffsets();
        
        // 强制同步所有组件的位置信息
        // 确保事件系统使用最新的位置数据
        rightPanel->setPosition(rightPanel->getX(), rightPanel->getY());
        okButton->setPosition(okButton->getX(), okButton->getY());
        
        // 或者添加一个强制刷新方法
        // 让所有组件重新计算并同步位置信息
        std::cout << "强制同步位置信息完成" << std::endl;
        
        // 强制更新rightPanel的位置
        rightPanel->setPosition(rightPanel->getX(), rightPanel->getY());
        
        std::cout << "\n=== OK按钮点击事件结束 ===\n" << std::endl;
    });
    
    // 右面板的按钮，添加动画效果
    auto scaleInButton = std::make_shared<UIButton>(0, 0, 110, 50, "SCALE IN");
    scaleInButton->setOnClick([&,texture,label, scaleInButton, rightPanel1, mainPanel, rightPanel]() {
       
       
       
        if (scalex > 0.2f) {
            scalex -= 0.2f;
            scaley -= 0.2f;  
        }
        else{
            scalex = 0.2f;
            scaley = 0.2f; 
        }

        UIAnimationManager::getInstance().scaleTo(texture.get(), 1.0f * scalex, 1.0f *scaley, 0.35f, UIAnimation::EASE_OUT);


        // === 执行布局更新 ===
        std::cout << "\n--- 开始布局更新 ---" << std::endl;
        mainPanel->updateLayout();
        
        // 使用递归方法重置所有组件的动画偏移
        // mainPanel->resetAllAnimationOffsets();
        
        // 强制同步所有组件的位置信息
        // 确保事件系统使用最新的位置数据
        rightPanel->setPosition(rightPanel->getX(), rightPanel->getY());
        okButton->setPosition(okButton->getX(), okButton->getY());
        
        // 或者添加一个强制刷新方法
        // 让所有组件重新计算并同步位置信息
        std::cout << "强制同步位置信息完成" << std::endl;
        
        // 强制更新rightPanel的位置
        rightPanel->setPosition(rightPanel->getX(), rightPanel->getY());
        
        std::cout << "\n=== OK按钮点击事件结束 ===\n" << std::endl;
    });



    // 事件处理 /////////////////////////////////////////////////////////////
    // // 设置拖拽回调
    static float initialX = rightPanel1->getX();
    static float initialY = rightPanel1->getY();
    static float totalDeltaX = 0.0f;
    static float totalDeltaY = 0.0f;
    
    texture->setOnDrag([&,rightPanel1,texture](float deltaX, float deltaY) {
        std::cout << "拖拽偏移: (" << deltaX << ", " << deltaY << ")" << std::endl;
        
        totalDeltaX += deltaX ;
        totalDeltaY += deltaY ;
        
        // texture->moveTo(initialX + totalDeltaX, initialY + totalDeltaY, 0.0f);
        // UIAnimationManager::getInstance().moveTo(texture.get(), initialX + totalDeltaX, initialY + totalDeltaY, 0.0f, UIAnimation::EASE_OUT);
        // mainPanel->updateLayout();
        rightPanel1->moveTo(totalDeltaX , totalDeltaY, 0.2f);
        // mainPanel->updateLayout();
        rightPanel1->updateLayout ();
    });
  
    // 设置滚动回调（需要在事件系统中添加滚动事件支持）
    texture->setOnScroll([&,texture](float scrollX, float scrollY) {
        std::cout << "滚动: (" << scrollX << ", " << scrollY << ")" << std::endl;
        scalex += scrollY*0.15;
        scaley += scrollY*0.15; 
        std::cout << "缩放: (" << scalex << ", " << scaley << ")" << std::endl;
        if (scalex >= 10.0f) {

            scalex = 10.0f;
            scaley = 10.0f; 
        }
        else if (scalex <= 0.2f) {
            scalex = 0.2f;
            scaley = 0.2f; 
        }

        UIAnimationManager::getInstance().scaleTo(texture.get(), 1.0f * scalex, 1.0f *scaley, 0.35f, UIAnimation::EASE_OUT);

    });

    // 设置按键回调
    texture->setOnKeyPress([](int keyCode, int modifiers) {
        std::cout << "按键: " << keyCode << ", 修饰键: " << modifiers << std::endl;
    });

    // 设置拖拽时滚轮回调
    texture->setOnDragScroll([&, texture, rightPanel1, mainPanel, rightPanel](float scrollX, float scrollY) {
        std::cout << "拖拽时滚轮: (" << scrollX << ", " << scrollY << ")" << std::endl;

        change_speed += scrollY;
        if (change_speed%2 != 0){
            return ;
        }

        current_index += change_speed / 2;
        change_speed = 0;

        if (current_index <= 0){
            current_index = 0;
            return;
        }
        if (current_index >= limit_index-1){
            current_index = limit_index-1;
            return;
        }
        
        imagPath =image_paths[current_index].generic_string();
        std::cout<< "last imagPath:" << imagPath <<std::endl;
        texture->setImagePath(window.getNVGContext(), imagPath);
        // mainPanel->updateLayout();
        rightPanel1->updateLayout ();
        
    });

    // 设置双击回调
    texture->setOnDoubleClick([](int mouseButton) {
        if (mouseButton == 0) {
            std::cout << "左键双击" << std::endl;
        } else if (mouseButton == 1) {
            std::cout << "右键双击" << std::endl;
        }
    });

    // 设置中键点击回调
    texture->setOnMiddleClick([&,texture](float mouseX, float mouseY) {
        std::cout << "Middle mouse button clicked at (" << mouseX << ", " << mouseY << ")" << std::endl;
        scalex = 1.0f;
        scaley = 1.0f; 
        UIAnimationManager::getInstance().scaleTo(texture.get(), 1.0f * scalex, 1.0f *scaley, 0.35f, UIAnimation::EASE_OUT);
    });

    // 启用所有事件
    texture->setDragEnabled(true);
    texture->setScrollEnabled(true);
    texture->setDoubleClickEnabled(true);
    texture->setDragScrollEnabled(true);
    texture->setMiddleClickEnabled(true);
    
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
    
    // next 图片切换
    auto nextButton = std::make_shared<UIButton>(0, 0, 70, 40, "Next");
    nextButton->setOnClick([&, nextButton,texture, rightPanel1, mainPanel, rightPanel]() {

        if (current_index == limit_index-1){
            return;
        }

        if (current_index < limit_index-1)    { current_index++; }

        imagPath =image_paths[current_index].generic_string();
        std::cout<< "next imagPath:" << imagPath <<std::endl;
        texture->setImagePath(window.getNVGContext(), imagPath);
        // mainPanel->updateLayout();
        rightPanel1->updateLayout ();

    });
    //last 图片切换
    auto lastButton = std::make_shared<UIButton>(0, 0, 70, 40, "Last");
    
    lastButton->setOnClick([&, nextButton,texture, lastButton, rightPanel1, mainPanel, rightPanel]() {
        
        rightPanel1->moveTo(100 , 100, 0.35f);
        // mainPanel->updateLayout();
        rightPanel1->updateLayout ();
    });

    rightPanel->addChild(okButton);
    rightPanel->addChild(scaleInButton);
    rightPanel->addChild(exitButton);

    rightPanel->addChild(nextButton);
    rightPanel->addChild(lastButton);

    rightPanel1->addChild(texture);
    rightPanel1->addChild(label);
    // 添加子面板到主面板
    // mainPanel->addChild(leftPanel);
    mainPanel->addChild(rightPanel1);
    // mainPanel->addChild(rightPanel);
 
    // printImagePath();

    // 设置鼠标事件回调
    // 设置鼠标事件回调
    window.setMouseButtonCallback([mainPanel](int button, int action, int mods) {
        // 处理所有鼠标按键，不只是左键
        double xpos, ypos;
        glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);
        
        UIEvent event;
        event.type = (action == GLFW_PRESS) ? UIEvent::MOUSE_PRESS : UIEvent::MOUSE_RELEASE;
        event.mouseX = static_cast<float>(xpos);
        event.mouseY = static_cast<float>(ypos);
        event.mouseButton = button;  // 直接传递button值
        
        // 添加时间戳用于双击检测
        event.clickTime = glfwGetTime();
        
        // 添加调试输出
        std::cout << "Mouse button " << button << " action " << action << std::endl;
        
        mainPanel->handleEvent(event);
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
    
    // 添加滚轮事件处理
    window.setScrollCallback([mainPanel](double xoffset, double yoffset) {
        UIEvent event;
        event.type = UIEvent::MOUSE_SCROLL;
        
        // 获取当前鼠标位置
        double xpos, ypos;
        glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);
        event.mouseX = static_cast<float>(xpos);
        event.mouseY = static_cast<float>(ypos);
        
        // 设置滚轮偏移量
        event.scrollX = static_cast<float>(xoffset);
        event.scrollY = static_cast<float>(yoffset);
        
        // 分发事件到主面板
        mainPanel->handleEvent(event);
    });
    
    // 在渲染前输出所有组件的位置和尺寸信息
    printAllComponentsInfo(mainPanel, leftPanel, rightPanel, label, button1, button2, button3, okButton, exitButton);
    
    // 主渲染循环
    auto lastTime = glfwGetTime();
    while (!window.shouldClose()) {
        window.pollEvents();
        // 每帧获取当前窗口大小
        int currentWidth, currentHeight;
        window.getFramebufferSize(currentWidth, currentHeight);
        // 计算时间差
        auto currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        // 更新动画系统
        UIAnimationManager::getInstance().update(deltaTime);
        
        window.beginFrame();
        window.clearBackground(1.0f, 1.0f, 1.0f, 0.90f);
        
        // 渲染主面板（会递归渲染所有子组件）
        mainPanel->updateLayout();
        mainPanel->render(window.getNVGContext());
        // texture->render(window.getNVGContext());
        window.endFrame();
        window.swapBuffers();
    }
    
    // 清理所有纹理资源
    UITexture::cleanupAll(window.getNVGContext());
    window.cleanup();
    return 0;
}
