#pragma once
#include "UIComponent.h"
#include <functional>
#include <string>
#include <vector>
#include <memory>

// 前向声明
class UICheckboxGroup;

/**
 * @class UICheckbox
 * @brief 复选框控件类
 * @description 可选中/取消选中的复选框组件，支持文字标签和状态变化事件
 */
class UICheckbox : public UIComponent {
public:
    enum Type {
        CHECKBOX,  // 复选框（可多选）
        RADIO      // 单选框（只能选一个）
    };
    
private:
    bool m_checked = false;
    std::string m_text;
    Type m_type = CHECKBOX;
    float m_fontSize = 14.0f;
    float m_checkboxSize = 16.0f;
    float m_textSpacing = 8.0f;
    
    // 组管理（用于单选框）
    UICheckboxGroup* m_group = nullptr;
    std::string m_groupName;
    
    // 颜色配置
    NVGcolor m_textColor = nvgRGB(255, 255, 255);
    NVGcolor m_checkColor = nvgRGB(0, 120, 215);
    NVGcolor m_hoverColor = nvgRGB(230, 230, 230);
    NVGcolor m_borderColor = nvgRGB(150, 150, 150);
    
    // 状态
    bool m_isHovered = false;
    bool m_isFocused = false;
    
    // 回调函数
    std::function<void(bool)> m_onStateChanged;
    
public:
    // 构造函数
    UICheckbox(float x, float y, float width, float height, const std::string& text = "", Type type = CHECKBOX);
    
    // 重写基类虚函数
    void render(NVGcontext* vg) override;
    void update(double deltaTime) override;
    bool handleEvent(const UIEvent& event) override;
    
    // 复选框特有接口
    void setChecked(bool checked);
    bool isChecked() const { return m_checked; }
    void toggle();
    
    void setText(const std::string& text) { m_text = text; }
    const std::string& getText() const { return m_text; }
    
    void setType(Type type) { m_type = type; }
    Type getType() const { return m_type; }
    
    // 组管理
    void setGroup(UICheckboxGroup* group) { m_group = group; }
    void setGroupName(const std::string& groupName) { m_groupName = groupName; }
    const std::string& getGroupName() const { return m_groupName; }
    
    void setOnStateChanged(std::function<void(bool)> callback) { m_onStateChanged = callback; }
    
    // 样式设置
    void setTextColor(NVGcolor color) { m_textColor = color; }
    void setCheckColor(NVGcolor color) { m_checkColor = color; }
    void setHoverColor(NVGcolor color) { m_hoverColor = color; }
    void setFontSize(float size) { m_fontSize = size; }
    void setCheckboxSize(float size) { m_checkboxSize = size; }
    
private:
    void renderCheckbox(NVGcontext* vg);
    void renderRadio(NVGcontext* vg);
    void renderText(NVGcontext* vg);
    bool isPointInCheckbox(float px, float py) const;
};

/**
 * @class UICheckboxGroup
 * @brief 复选框组管理类
 * @description 用于管理单选框组，确保同一组内只有一个被选中
 */
class UICheckboxGroup {
private:
    std::vector<UICheckbox*> m_checkboxes;
    std::string m_groupName;
    UICheckbox* m_selectedCheckbox = nullptr;
    
public:
    UICheckboxGroup(const std::string& groupName) : m_groupName(groupName) {}
    
    void addCheckbox(UICheckbox* checkbox);
    void removeCheckbox(UICheckbox* checkbox);
    void selectCheckbox(UICheckbox* checkbox);
    UICheckbox* getSelectedCheckbox() const { return m_selectedCheckbox; }
    const std::string& getGroupName() const { return m_groupName; }
};