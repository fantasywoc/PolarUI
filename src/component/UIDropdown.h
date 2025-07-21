#pragma once
#include "UIComponent.h"
#include <functional>
#include <vector>
#include <string>

/**
 * @class UIDropdown
 * @brief 下拉列表控件类
 * @description 下拉选择组件，支持多个选项和选择事件
 */
class UIDropdown : public UIComponent {
public:
    struct DropdownItem {
        std::string text;
        bool enabled = true;
        
        DropdownItem(const std::string& t, bool e = true) : text(t), enabled(e) {}
    };
    
private:
    std::vector<DropdownItem> m_items;
    int m_selectedIndex = -1;
    bool m_isOpen = false;
    float m_itemHeight = 24.0f;
    float m_maxDropdownHeight = 200.0f;
    int m_hoveredIndex = -1;
    
    // 颜色配置
    NVGcolor m_textColor = nvgRGB(255, 255, 255);
    NVGcolor m_selectedColor = nvgRGB(0, 120, 215);
    NVGcolor m_hoverColor = nvgRGB(230, 230, 230);
    NVGcolor m_dropdownBg = nvgRGB(50, 50, 50);
    NVGcolor m_arrowColor = nvgRGB(150, 150, 150);
    
    // 状态
    bool m_isHovered = false;
    float m_fontSize = 14.0f;
    
    // 回调函数
    std::function<void(int, const std::string&)> m_onSelectionChanged;
    
public:
    // 构造函数
    UIDropdown(float x, float y, float width, float height = 30);
    
    // 重写基类虚函数
    void render(NVGcontext* vg) override;
    void update(double deltaTime) override;
    bool handleEvent(const UIEvent& event) override;
    
    // 下拉列表特有接口
    void addItem(const std::string& text, bool enabled = true);
    void removeItem(int index);
    void clearItems();
    
    void setSelectedIndex(int index);
    int getSelectedIndex() const { return m_selectedIndex; }
    std::string getSelectedText() const;
    
    void setOnSelectionChanged(std::function<void(int, const std::string&)> callback) {
        m_onSelectionChanged = callback;
    }
    
    // 样式设置
    void setTextColor(NVGcolor color) { m_textColor = color; }
    void setSelectedColor(NVGcolor color) { m_selectedColor = color; }
    void setHoverColor(NVGcolor color) { m_hoverColor = color; }
    void setFontSize(float size) { m_fontSize = size; }
    void setItemHeight(float height) { m_itemHeight = height; }
    
    bool isOpen() const { return m_isOpen; }
    void setOpen(bool open) { m_isOpen = open; }
    
private:
    void renderMainButton(NVGcontext* vg);
    void renderDropdownList(NVGcontext* vg);
    void renderArrow(NVGcontext* vg);
    bool isPointInDropdown(float px, float py) const;
    int getItemIndexAtPoint(float px, float py) const;
    float getDropdownHeight() const;
};