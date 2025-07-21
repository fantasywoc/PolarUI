#include "UICheckbox.h"
#include <iostream>
#include <algorithm>

// UICheckboxGroup 实现
void UICheckboxGroup::addCheckbox(UICheckbox* checkbox) {
    if (checkbox && std::find(m_checkboxes.begin(), m_checkboxes.end(), checkbox) == m_checkboxes.end()) {
        m_checkboxes.push_back(checkbox);
        checkbox->setGroup(this);
    }
}

void UICheckboxGroup::removeCheckbox(UICheckbox* checkbox) {
    auto it = std::find(m_checkboxes.begin(), m_checkboxes.end(), checkbox);
    if (it != m_checkboxes.end()) {
        m_checkboxes.erase(it);
        if (m_selectedCheckbox == checkbox) {
            m_selectedCheckbox = nullptr;
        }
    }
}

void UICheckboxGroup::selectCheckbox(UICheckbox* checkbox) {
    if (checkbox && std::find(m_checkboxes.begin(), m_checkboxes.end(), checkbox) != m_checkboxes.end()) {
        // 取消其他选中状态
        for (auto* cb : m_checkboxes) {
            if (cb != checkbox && cb->isChecked()) {
                cb->setChecked(false);
            }
        }
        m_selectedCheckbox = checkbox;
    }
}

// UICheckbox 实现
UICheckbox::UICheckbox(float x, float y, float width, float height, const std::string& text, Type type)
    : UIComponent(x, y, width, height), m_text(text), m_type(type) {
    m_backgroundColor = nvgRGBA(0, 0, 0, 0); // 透明背景
}

void UICheckbox::render(NVGcontext* vg) {
    if (!m_visible) return;
    
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
    
    nvgGlobalAlpha(vg, m_animationOpacity);
    
    // 根据类型渲染不同样式
    if (m_type == RADIO) {
        renderRadio(vg);
    } else {
        renderCheckbox(vg);
    }
    renderText(vg);
    
    nvgRestore(vg);
}

void UICheckbox::update(double deltaTime) {
    // 可以添加动画效果或其他更新逻辑
}

bool UICheckbox::handleEvent(const UIEvent& event) {
    if (!m_visible || !m_enabled) return false;
    
    bool wasHovered = m_isHovered;
    
    switch (event.type) {
        case UIEvent::MOUSE_MOVE:
            m_isHovered = contains(event.mouseX, event.mouseY);
            return m_isHovered;
            
        case UIEvent::MOUSE_PRESS:
            if (m_isHovered && event.mouseButton == 0) {
                m_isFocused = true;
                return true;
            } else {
                m_isFocused = false;
            }
            break;
            
        case UIEvent::MOUSE_RELEASE:
            if (m_isHovered && m_isFocused && event.mouseButton == 0) {
                toggle();
                return true;
            }
            break;
    }
    
    return false;
}

void UICheckbox::toggle() {
    if (m_type == RADIO) {
        // 单选框只能选中，不能取消
        if (!m_checked) {
            setChecked(true);
        }
    } else {
        // 复选框可以切换
        setChecked(!m_checked);
    }
}

void UICheckbox::setChecked(bool checked) {
    if (m_checked != checked) {
        m_checked = checked;
        
        // 如果是单选框且被选中，通知组管理器
        if (m_type == RADIO && checked && m_group) {
            m_group->selectCheckbox(this);
        }
        
        if (m_onStateChanged) {
            m_onStateChanged(m_checked);
        }
    }
}

void UICheckbox::renderCheckbox(NVGcontext* vg) {
    float checkboxY = (m_height - m_checkboxSize) / 2.0f;
    
    // 绘制复选框背景
    nvgBeginPath(vg);
    nvgRoundedRect(vg, 2, checkboxY, m_checkboxSize, m_checkboxSize, 2);
    
    if (m_isHovered) {
        nvgFillColor(vg, m_hoverColor);
    } else {
        nvgFillColor(vg, nvgRGB(255, 255, 255));
    }
    nvgFill(vg);
    
    // 绘制边框
    nvgStrokeColor(vg, m_borderColor);
    nvgStrokeWidth(vg, 1.0f);
    nvgStroke(vg);
    
    // 绘制选中标记
    if (m_checked) {
        nvgBeginPath(vg);
        nvgMoveTo(vg, 5, checkboxY + m_checkboxSize/2);
        nvgLineTo(vg, 8, checkboxY + m_checkboxSize - 4);
        nvgLineTo(vg, 14, checkboxY + 4);
        nvgStrokeColor(vg, m_checkColor);
        nvgStrokeWidth(vg, 2.0f);
        nvgStroke(vg);
    }
}

void UICheckbox::renderText(NVGcontext* vg) {
    if (m_text.empty()) return;
    
    nvgFontFace(vg, "default");
    nvgFontSize(vg, m_fontSize);
    nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    nvgFillColor(vg, m_textColor);
    
    float textX = m_checkboxSize + m_textSpacing;
    float textY = m_height / 2.0f;
    nvgText(vg, textX, textY, m_text.c_str(), nullptr);
}

bool UICheckbox::isPointInCheckbox(float px, float py) const {
    float checkboxY = (m_height - m_checkboxSize) / 2.0f;
    return px >= m_x && px <= m_x + m_checkboxSize && 
           py >= m_y + checkboxY && py <= m_y + checkboxY + m_checkboxSize;
}

void UICheckbox::renderRadio(NVGcontext* vg) {
    float radioY = (m_height - m_checkboxSize) / 2.0f;
    float centerX = 2 + m_checkboxSize / 2.0f;
    float centerY = radioY + m_checkboxSize / 2.0f;
    float radius = m_checkboxSize / 2.0f;
    
    // 绘制圆形背景
    nvgBeginPath(vg);
    nvgCircle(vg, centerX, centerY, radius);
    
    if (m_isHovered) {
        nvgFillColor(vg, m_hoverColor);
    } else {
        nvgFillColor(vg, nvgRGB(255, 255, 255));
    }
    nvgFill(vg);
    
    // 绘制边框
    nvgStrokeColor(vg, m_borderColor);
    nvgStrokeWidth(vg, 1.0f);
    nvgStroke(vg);
    
    // 绘制选中标记（实心圆）
    if (m_checked) {
        nvgBeginPath(vg);
        nvgCircle(vg, centerX, centerY, radius - 4);
        nvgFillColor(vg, m_checkColor);
        nvgFill(vg);
    }
}