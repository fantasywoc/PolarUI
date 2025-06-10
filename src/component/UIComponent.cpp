#include "UIComponent.h"
#include "../animation/UIAnimationManager.h"

UIComponent::UIComponent(float x, float y, float width, float height)
    : m_x(x), m_y(y), m_width(width), m_height(height), 
      m_visible(true), m_enabled(true), m_display(true) {
}

void UIComponent::setPosition(float x, float y) {
    m_x = x;
    m_y = y;
}

void UIComponent::setSize(float width, float height) {
    m_width = width;
    m_height = height;
}

void UIComponent::setBounds(float x, float y, float width, float height) {
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
}

bool UIComponent::contains(float px, float py) const {
    // 考虑动画偏移的实际位置
    float actualX = m_x + m_animationOffsetX;
    float actualY = m_y + m_animationOffsetY;
    return px >= actualX && px <= actualX + m_width && py >= actualY && py <= actualY + m_height;
}

void UIComponent::renderBackground(NVGcontext* vg) {
    nvgBeginPath(vg);
    nvgRoundedRect(vg, m_x, m_y, m_width, m_height, m_cornerRadius);
    nvgFillColor(vg, m_backgroundColor);
    nvgFill(vg);
}

void UIComponent::renderBorder(NVGcontext* vg) {
    if (m_borderWidth > 0) {
        nvgBeginPath(vg);
        nvgRoundedRect(vg, m_x, m_y, m_width, m_height, m_cornerRadius);
        nvgStrokeColor(vg, m_borderColor);
        nvgStrokeWidth(vg, m_borderWidth);
        nvgStroke(vg);
    }
}

// 动画接口实现 - 使用UIAnimationManager的便捷方法
// 移除错误的 render 方法实现，只保留以下动画接口方法：

void UIComponent::fadeIn(float duration, UIAnimation::EasingType easing) {
    UIAnimationManager::getInstance().fadeIn(this, duration, easing);
}

void UIComponent::fadeOut(float duration, UIAnimation::EasingType easing) {
    UIAnimationManager::getInstance().fadeOut(this, duration, easing);
}

void UIComponent::moveTo(float x, float y, float duration) {
    UIAnimationManager::getInstance().moveTo(this, x, y, duration);
}

void UIComponent::scaleTo(float scaleX, float scaleY, float duration) {
    UIAnimationManager::getInstance().scaleTo(this, scaleX, scaleY, duration);
}

void UIComponent::rotateTo(float angle, float duration) {
    UIAnimationManager::getInstance().rotateTo(this, angle, duration);
}
void UIComponent::render(NVGcontext* vg) {
    if (!m_visible || m_animationOpacity <= 0.0f) return;
    
    nvgSave(vg);
    
    // 应用动画变换
    nvgTranslate(vg, m_x + m_animationOffsetX, m_y + m_animationOffsetY);
    
    if (m_animationRotation != 0.0f) {
        nvgTranslate(vg, m_width/2, m_height/2);
        nvgRotate(vg, m_animationRotation);
        nvgTranslate(vg, -m_width/2, -m_height/2);
    }
    
    if (m_animationScaleX != 1.0f || m_animationScaleY != 1.0f) {
        nvgTranslate(vg, m_width/2, m_height/2);
        nvgScale(vg, m_animationScaleX, m_animationScaleY);
        nvgTranslate(vg, -m_width/2, -m_height/2);
    }
    
    // 应用透明度
    nvgGlobalAlpha(vg, m_animationOpacity);
    
    // 调用具体组件的渲染逻辑
    renderContent(vg);
    
    nvgRestore(vg);
}