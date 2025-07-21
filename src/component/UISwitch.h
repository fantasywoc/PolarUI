#pragma once
#include "UIComponent.h"
#include <functional>
#include <algorithm>

/**
 * @class UISwitch
 * @brief 开关控件类
 * @description 滑动开关组件，支持开/关状态切换和动画效果
 */
class UISwitch : public UIComponent {
private:
    bool m_isOn = false;
    float m_switchWidth = 50.0f;
    float m_switchHeight = 26.0f;
    float m_knobSize = 20.0f;
    float m_animationProgress = 0.0f; // 0.0 = off, 1.0 = on
    
    // 颜色配置
    NVGcolor m_onColor = nvgRGB(0, 200, 0);      // 开启时绿色背景
    NVGcolor m_offColor = nvgRGB(255, 255, 255); // 关闭时白色背景
    NVGcolor m_knobColor = nvgRGB(255, 255, 255);
    NVGcolor m_hoverColor = nvgRGB(200, 200, 200);
    
    // 状态
    bool m_isHovered = false;
    bool m_isDragging = false;
    float m_dragStartX = 0.0f;
    
    // 回调函数
    std::function<void(bool)> m_onStateChanged;
    
public:
    // 构造函数
    UISwitch(float x, float y, bool initialState = false);
    
    // 重写基类虚函数
    void render(NVGcontext* vg) override;
    void update(double deltaTime) override;
    bool handleEvent(const UIEvent& event) override;
    
    // 开关特有接口
    void setOn(bool on);
    bool isOn() const { return m_isOn; }
    void toggle() { setOn(!m_isOn); }
    
    void setOnStateChanged(std::function<void(bool)> callback) { m_onStateChanged = callback; }
    
    // 样式设置
    void setOnColor(NVGcolor color) { m_onColor = color; }
    void setOffColor(NVGcolor color) { m_offColor = color; }
    void setKnobColor(NVGcolor color) { m_knobColor = color; }
    
private:
    void updateAnimation(double deltaTime);
    NVGcolor getCurrentTrackColor() const;
    float getKnobPosition() const;
};