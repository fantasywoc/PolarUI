#include "UILabel.h"
#include <nanovg.h>

UILabel::UILabel(float x, float y, float width, float height, const std::string& text)
    : UIComponent(x, y, width, height), m_text(text) {
}

void UILabel::render(NVGcontext* vg) {
    if (!m_visible) return;
    
    nvgSave(vg);
    
    // 应用动画变换
    nvgTranslate(vg, m_x + m_animationOffsetX, m_y + m_animationOffsetY);
    nvgRotate(vg, m_animationRotation);
    nvgScale(vg, m_animationScaleX, m_animationScaleY);
    
    // 应用透明度
    nvgGlobalAlpha(vg, m_animationOpacity);
    
    // 渲染文字（需要修改 renderText 方法使用相对坐标）
    renderText(vg);
    
    nvgRestore(vg);
}

void UILabel::update(double deltaTime) {
    // UILabel 通常不需要更新逻辑
}

bool UILabel::handleEvent(const UIEvent& event) {
    // UILabel 通常不处理事件
    return false;
}

void UILabel::autoResize(NVGcontext* vg) {
    if (m_text.empty()) return;
    
    nvgFontSize(vg, m_fontSize);
    nvgTextAlign(vg, m_textAlign | m_verticalAlign);
    
    float bounds[4];
    nvgTextBounds(vg, 0, 0, m_text.c_str(), nullptr, bounds);
    
    m_width = bounds[2] - bounds[0];
    m_height = bounds[3] - bounds[1];
}

void UILabel::renderText(NVGcontext* vg) {
    if (m_text.empty()) return;
    
    nvgFontSize(vg, m_fontSize);
    nvgFillColor(vg, m_textColor);
    nvgTextAlign(vg, m_textAlign | m_verticalAlign);
    
    float textX = 0; // 使用相对坐标
    float textY = 0; // 使用相对坐标
    
    // 根据对齐方式调整文字位置
    if (m_textAlign == CENTER) {
        textX += m_width / 2.0f;
    } else if (m_textAlign == RIGHT) {
        textX += m_width;
    }
    
    if (m_verticalAlign == MIDDLE) {
        textY += m_height / 2.0f;
    } else if (m_verticalAlign == BOTTOM) {
        textY += m_height;
    }
    
    nvgText(vg, textX, textY, m_text.c_str(), nullptr);
}