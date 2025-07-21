#include "UIDropdown.h"
#include <algorithm>
#include <iostream>

UIDropdown::UIDropdown(float x, float y, float width, float height)
    : UIComponent(x, y, width, height) {
    m_backgroundColor = nvgRGB(70, 70, 70);
    m_borderColor = nvgRGB(100, 100, 100);
    m_borderWidth = 1.0f;
    m_cornerRadius = 4.0f;
}

void UIDropdown::render(NVGcontext* vg) {
    if (!m_visible) return;
    
    nvgSave(vg);
    
    // 应用动画变换
    nvgTranslate(vg, m_x + m_animationOffsetX, m_y + m_animationOffsetY);
    nvgGlobalAlpha(vg, m_animationOpacity);
    
    // 渲染主按钮
    renderMainButton(vg);
    
    // 渲染下拉列表（如果打开）
    if (m_isOpen) {
        renderDropdownList(vg);
    }
    
    nvgRestore(vg);
}

void UIDropdown::update(double deltaTime) {
    // 可以添加动画效果
}

bool UIDropdown::handleEvent(const UIEvent& event) {
    if (!m_visible || !m_enabled) return false;
    
    switch (event.type) {
        case UIEvent::MOUSE_MOVE:
            if (m_isOpen) {
                m_hoveredIndex = getItemIndexAtPoint(event.mouseX, event.mouseY);
            }
            m_isHovered = contains(event.mouseX, event.mouseY) || 
                         (m_isOpen && isPointInDropdown(event.mouseX, event.mouseY));
            return m_isHovered;
            
        case UIEvent::MOUSE_PRESS:
            if (event.mouseButton == 0) {
                if (contains(event.mouseX, event.mouseY)) {
                    // 点击主按钮
                    m_isOpen = !m_isOpen;
                    return true;
                } else if (m_isOpen && isPointInDropdown(event.mouseX, event.mouseY)) {
                    // 点击下拉列表项
                    int itemIndex = getItemIndexAtPoint(event.mouseX, event.mouseY);
                    if (itemIndex >= 0 && itemIndex < m_items.size() && m_items[itemIndex].enabled) {
                        setSelectedIndex(itemIndex);
                        m_isOpen = false;
                    }
                    return true;
                } else if (m_isOpen) {
                    // 点击外部，关闭下拉列表
                    m_isOpen = false;
                    return false;
                }
            }
            break;
    }
    
    return false;
}

void UIDropdown::addItem(const std::string& text, bool enabled) {
    m_items.emplace_back(text, enabled);
}

void UIDropdown::removeItem(int index) {
    if (index >= 0 && index < m_items.size()) {
        m_items.erase(m_items.begin() + index);
        if (m_selectedIndex == index) {
            m_selectedIndex = -1;
        } else if (m_selectedIndex > index) {
            m_selectedIndex--;
        }
    }
}

void UIDropdown::clearItems() {
    m_items.clear();
    m_selectedIndex = -1;
}

void UIDropdown::setSelectedIndex(int index) {
    if (index >= -1 && index < m_items.size()) {
        int oldIndex = m_selectedIndex;
        m_selectedIndex = index;
        if (oldIndex != m_selectedIndex && m_onSelectionChanged) {
            m_onSelectionChanged(m_selectedIndex, getSelectedText());
        }
    }
}

std::string UIDropdown::getSelectedText() const {
    if (m_selectedIndex >= 0 && m_selectedIndex < m_items.size()) {
        return m_items[m_selectedIndex].text;
    }
    return "";
}

void UIDropdown::renderMainButton(NVGcontext* vg) {
    // 绘制主按钮背景
    nvgBeginPath(vg);
    nvgRoundedRect(vg, 0, 0, m_width, m_height, m_cornerRadius);
    nvgFillColor(vg, m_isHovered ? m_hoverColor : m_backgroundColor);
    nvgFill(vg);
    
    // 绘制边框
    nvgStrokeColor(vg, m_borderColor);
    nvgStrokeWidth(vg, m_borderWidth);
    nvgStroke(vg);
    
    // 绘制选中的文本
    nvgFontFace(vg, "default");
    nvgFontSize(vg, m_fontSize);
    nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    nvgFillColor(vg, m_textColor);
    
    std::string displayText = getSelectedText();
    if (displayText.empty()) {
        displayText = "请选择...";
    }
    
    nvgText(vg, 8, m_height/2, displayText.c_str(), nullptr);
    
    // 绘制箭头
    renderArrow(vg);
}

void UIDropdown::renderDropdownList(NVGcontext* vg) {
    float dropdownHeight = getDropdownHeight();
    float dropdownY = m_height;
    
    // 绘制下拉列表背景
    nvgBeginPath(vg);
    nvgRoundedRect(vg, 0, dropdownY, m_width, dropdownHeight, m_cornerRadius);
    nvgFillColor(vg, m_dropdownBg);
    nvgFill(vg);
    
    // 绘制边框
    nvgStrokeColor(vg, m_borderColor);
    nvgStrokeWidth(vg, m_borderWidth);
    nvgStroke(vg);
    
    // 绘制列表项
    nvgFontFace(vg, "default");
    nvgFontSize(vg, m_fontSize);
    nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    
    for (int i = 0; i < m_items.size(); ++i) {
        float itemY = dropdownY + i * m_itemHeight;
        
        // 绘制项目背景（悬停或选中）
        if (i == m_hoveredIndex || i == m_selectedIndex) {
            nvgBeginPath(vg);
            nvgRect(vg, 2, itemY + 1, m_width - 4, m_itemHeight - 2);
            nvgFillColor(vg, i == m_selectedIndex ? m_selectedColor : m_hoverColor);
            nvgFill(vg);
        }
        
        // 绘制文本
        NVGcolor textColor = m_items[i].enabled ? m_textColor : nvgRGB(100, 100, 100);
        nvgFillColor(vg, textColor);
        nvgText(vg, 8, itemY + m_itemHeight/2, m_items[i].text.c_str(), nullptr);
    }
}

void UIDropdown::renderArrow(NVGcontext* vg) {
    float arrowSize = 6.0f;
    float arrowX = m_width - 16;
    float arrowY = m_height / 2;
    
    nvgBeginPath(vg);
    if (m_isOpen) {
        // 向上箭头
        nvgMoveTo(vg, arrowX - arrowSize/2, arrowY + 2);
        nvgLineTo(vg, arrowX, arrowY - 2);
        nvgLineTo(vg, arrowX + arrowSize/2, arrowY + 2);
    } else {
        // 向下箭头
        nvgMoveTo(vg, arrowX - arrowSize/2, arrowY - 2);
        nvgLineTo(vg, arrowX, arrowY + 2);
        nvgLineTo(vg, arrowX + arrowSize/2, arrowY - 2);
    }
    nvgStrokeColor(vg, m_arrowColor);
    nvgStrokeWidth(vg, 2.0f);
    nvgStroke(vg);
}

bool UIDropdown::isPointInDropdown(float px, float py) const {
    if (!m_isOpen) return false;
    
    float dropdownHeight = getDropdownHeight();
    return px >= m_x && px <= m_x + m_width && 
           py >= m_y + m_height && py <= m_y + m_height + dropdownHeight;
}

int UIDropdown::getItemIndexAtPoint(float px, float py) const {
    if (!isPointInDropdown(px, py)) return -1;
    
    float relativeY = py - (m_y + m_height);
    int index = (int)(relativeY / m_itemHeight);
    return (index >= 0 && index < m_items.size()) ? index : -1;
}

float UIDropdown::getDropdownHeight() const {
    float totalHeight = m_items.size() * m_itemHeight;
    return std::min(totalHeight, m_maxDropdownHeight);
}