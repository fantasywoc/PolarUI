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