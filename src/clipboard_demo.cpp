#include "UIWindow.h"
#include "component/UIButton.h"
#include "component/UILabel.h"
#include "component/UIPanel.h"
#include <windows.h>  // Windows剪切板API
#include <iostream>
#include <string>

class ClipboardManager {
public:
    // 获取剪切板内容
    static std::string getClipboardText() {
        if (!OpenClipboard(nullptr)) return "";
        
        HANDLE hData = GetClipboardData(CF_TEXT);
        if (hData == nullptr) {
            CloseClipboard();
            return "";
        }
        
        char* pszText = static_cast<char*>(GlobalLock(hData));
        if (pszText == nullptr) {
            CloseClipboard();
            return "";
        }
        
        std::string text(pszText);
        GlobalUnlock(hData);
        CloseClipboard();
        return text;
    }
    
    // 设置剪切板内容
    static bool setClipboardText(const std::string& text) {
        if (!OpenClipboard(nullptr)) return false;
        
        EmptyClipboard();
        
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, text.length() + 1);
        if (hMem == nullptr) {
            CloseClipboard();
            return false;
        }
        
        memcpy(GlobalLock(hMem), text.c_str(), text.length() + 1);
        GlobalUnlock(hMem);
        
        SetClipboardData(CF_TEXT, hMem);
        CloseClipboard();
        return true;
    }
};

int main() {
    UIWindow window(600, 400, "clipboard demo");
    if (!window.initialize()) return -1;
    
    // 创建主面板
    auto mainPanel = std::make_shared<UIPanel>(20, 20, 560, 360);
    
    // 标题
    auto titleLabel = std::make_shared<UILabel>(0, 10, 560, 30, "clipboard demo");
    titleLabel->setTextAlign(UILabel::CENTER);
    titleLabel->setFontSize(20);
    
    // 剪切板内容显示区域
    auto contentLabel = std::make_shared<UILabel>(20, 60, 520, 200, "clipboard demo content...");
    contentLabel->setBackgroundColor(nvgRGB(240, 240, 240));
    contentLabel->setBorderColor(nvgRGB(200, 200, 200));
    contentLabel->setBorderWidth(1);
    
    // 功能按钮
    auto refreshBtn = std::make_shared<UIButton>(50, 280, 100, 40, "update");
    auto copyBtn = std::make_shared<UIButton>(170, 280, 100, 40, "copy");
    auto clearBtn = std::make_shared<UIButton>(290, 280, 100, 40, "clear");
    auto exitBtn = std::make_shared<UIButton>(410, 280, 100, 40, "exit");
    
    // 绑定事件
    refreshBtn->setOnClick([&contentLabel]() {
        std::string clipText = ClipboardManager::getClipboardText();
        if (clipText.empty()) {
            contentLabel->setText("剪切板为空");
        } else {
            contentLabel->setText(clipText);
        }
    });
    
    copyBtn->setOnClick([&contentLabel]() {
        std::string currentText = contentLabel->getText();
        if (ClipboardManager::setClipboardText(currentText)) {
            std::cout << "已复制到剪切板" << std::endl;
        }
    });
    
    clearBtn->setOnClick([&contentLabel]() {
        ClipboardManager::setClipboardText("");
        contentLabel->setText("剪切板已清空");
    });
    
    exitBtn->setOnClick([&window]() {
        // 设置窗口关闭标志
        glfwSetWindowShouldClose(window.getGLFWWindow(), GLFW_TRUE);
    });
    
    // 添加组件到面板
    mainPanel->addChild(titleLabel);
    mainPanel->addChild(contentLabel);
    mainPanel->addChild(refreshBtn);
    mainPanel->addChild(copyBtn);
    mainPanel->addChild(clearBtn);
    mainPanel->addChild(exitBtn);
    
    // 主循环
    while (!window.shouldClose()) {
        window.beginFrame();
        window.clearBackground();
        
        mainPanel->update(0.016);
        mainPanel->render(window.getNVGContext());
        
        window.endFrame();
        window.swapBuffers();
        window.pollEvents();
    }
    
    return 0;
}