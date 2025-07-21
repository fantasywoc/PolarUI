#include "UISwitch.h"
#include <algorithm>
#include <iostream>

UISwitch::UISwitch(float x, float y, bool initialState)
    : UIComponent(x, y, 50, 26), m_isOn(initialState) {
    m_animationProgress = initialState ? 1.0f : 0.0f;
    m_backgroundColor = nvgRGBA(0, 0, 0, 0);
    // 确保组件尺寸与开关尺寸一致
    m_width = m_switchWidth;
    m_height = m_switchHeight;
}

void UISwitch::render(NVGcontext* vg) {
    if (!m_visible) return;
    
    nvgSave(vg);
    
    // 应用动画变换
    nvgTranslate(vg, m_x + m_animationOffsetX, m_y + m_animationOffsetY);
    nvgGlobalAlpha(vg, m_animationOpacity);
    
    // 绘制轨道背景
    nvgBeginPath(vg);
    nvgRoundedRect(vg, 0, 0, m_switchWidth, m_switchHeight, m_switchHeight/2);
    nvgFillColor(vg, getCurrentTrackColor());
    nvgFill(vg);
    
    // 添加边框（让白色背景可见）
    nvgStrokeColor(vg, nvgRGB(200, 200, 200));
    nvgStrokeWidth(vg, 1.0f);
    nvgStroke(vg);
    
    // 绘制旋钮
    float knobX = getKnobPosition();
    float knobY = (m_switchHeight - m_knobSize) / 2.0f;
    
    // 旋钮阴影
    nvgBeginPath(vg);
    nvgCircle(vg, knobX + m_knobSize/2 + 1, knobY + m_knobSize/2 + 1, m_knobSize/2 - 1);
    nvgFillColor(vg, nvgRGBA(0, 0, 0, 30));
    nvgFill(vg);
    
    // 旋钮主体
    nvgBeginPath(vg);
    nvgCircle(vg, knobX + m_knobSize/2, knobY + m_knobSize/2, m_knobSize/2 - 1);
    nvgFillColor(vg, m_knobColor);
    nvgFill(vg);
    
    // 旋钮边框
    nvgStrokeColor(vg, nvgRGB(180, 180, 180));
    nvgStrokeWidth(vg, 0.5f);
    nvgStroke(vg);
    
    nvgRestore(vg);
}

void UISwitch::update(double deltaTime) {
    updateAnimation(deltaTime);
}

bool UISwitch::handleEvent(const UIEvent& event) {
    if (!m_visible || !m_enabled) return false;
    
    switch (event.type) {
        case UIEvent::MOUSE_MOVE:
            m_isHovered = contains(event.mouseX, event.mouseY);
            return m_isHovered;
            
        case UIEvent::MOUSE_PRESS:
            if (m_isHovered && event.mouseButton == 0) {
                std::cout << "UISwitch: Mouse pressed at (" << event.mouseX << ", " << event.mouseY << ")" << std::endl;
                m_isDragging = true;
                m_dragStartX = event.mouseX;
                return true;
            }
            break;
            
        case UIEvent::MOUSE_RELEASE:
            if (event.mouseButton == 0) {
                std::cout << "UISwitch: Mouse released at (" << event.mouseX << ", " << event.mouseY << ")" << std::endl;
                std::cout << "UISwitch: m_isDragging=" << m_isDragging << ", m_isHovered=" << m_isHovered << std::endl;
                
                if (m_isDragging) {
                    m_isDragging = false;
                    // 拖拽后的点击切换
                    float dragDistance = abs(event.mouseX - m_dragStartX);
                    std::cout << "UISwitch: Drag distance=" << dragDistance << std::endl;
                    if (dragDistance < 5) {
                        std::cout << "UISwitch: Executing toggle()" << std::endl;
                        toggle();
                    }
                    return true;
                } else if (m_isHovered) {
                    // 简单点击切换
                    std::cout << "UISwitch: Simple click, executing toggle()" << std::endl;
                    toggle();
                    return true;
                }
            }
            break;
    }
    
    return false;
}

void UISwitch::setOn(bool on) {
    std::cout << "UISwitch::setOn() called, changing from " << m_isOn << " to " << on << std::endl;
    if (m_isOn != on) {
        m_isOn = on;
        std::cout << "UISwitch: State changed to " << (m_isOn ? "ON" : "OFF") << std::endl;
        if (m_onStateChanged) {
            std::cout << "UISwitch: Calling state change callback" << std::endl;
            m_onStateChanged(m_isOn);
        }
    } else {
        std::cout << "UISwitch: State unchanged, still " << (m_isOn ? "ON" : "OFF") << std::endl;
    }
}

void UISwitch::updateAnimation(double deltaTime) {
    float target = m_isOn ? 1.0f : 0.0f;
    float speed = 8.0f; // 动画速度
    
    float oldProgress = m_animationProgress;
    
    if (abs(m_animationProgress - target) > 0.01f) {
        if (m_animationProgress < target) {
            m_animationProgress = std::min(target, m_animationProgress + static_cast<float>(speed * deltaTime));
        } else {
            m_animationProgress = std::max(target, m_animationProgress - static_cast<float>(speed * deltaTime));
        }
        
        // 只在动画进度有明显变化时输出调试信息
        if (abs(m_animationProgress - oldProgress) > 0.05f) {
            std::cout << "UISwitch: Animation progress from " << oldProgress << " to " << m_animationProgress 
                      << ", target=" << target << ", deltaTime=" << deltaTime << std::endl;
        }
    } else {
        m_animationProgress = target;
    }
}

NVGcolor UISwitch::getCurrentTrackColor() const {
    // 在开启和关闭颜色之间插值
    float t = m_animationProgress;
    return nvgLerpRGBA(m_offColor, m_onColor, t);
}

float UISwitch::getKnobPosition() const {
    float maxPos = m_switchWidth - m_knobSize;
    return m_animationProgress * maxPos;
}