#include "FlexLayout.h"
#include "UIComponent.h"
#include <algorithm>
#include <iostream>

void FlexLayout::updateLayout(const std::vector<std::shared_ptr<UIComponent>>& children,
                             float containerX, float containerY,
                             float containerWidth, float containerHeight) {
    if (children.empty()) return;
    
    if (m_direction == HORIZONTAL) {
        layoutHorizontal(children, containerX, containerY, containerWidth, containerHeight);
    } else {
        layoutVertical(children, containerX, containerY, containerWidth, containerHeight);
    }
}

void FlexLayout::layoutHorizontal(const std::vector<std::shared_ptr<UIComponent>>& children,
                                 float containerX, float containerY,
                                 float containerWidth, float containerHeight) {
    if (children.empty()) return;
    
    float availableWidth = containerWidth - 2 * m_padding;
    float availableHeight = containerHeight - 2 * m_padding;
    
    // 计算所有子组件的总宽度
    float totalChildWidth = 0;
    for (auto& child : children) {
        if (child) {
            totalChildWidth += child->getWidth();
        }
    }
    
    // 计算起始X位置
    float startX = containerX + m_padding;
    float spacing = m_spacing;
    
    // 根据X轴对齐方式计算起始位置和间距
    switch (m_xAlignment) {
        case X_START:
            // 左对齐，使用默认spacing
            break;
        case X_CENTER: {
            // 居中对齐
            float totalWidth = totalChildWidth + (children.size() - 1) * spacing;
            float remainingWidth = availableWidth - totalWidth;
            startX += remainingWidth / 2.0f;
            break;
        }
        case X_END: {
            // 右对齐
            float totalWidth = totalChildWidth + (children.size() - 1) * spacing;
            float remainingWidth = availableWidth - totalWidth;
            startX += remainingWidth;
            break;
        }
    }
    
    // 布局所有子组件
    float currentX = startX;
    for (auto& child : children) {
        if (child) {
            // 计算Y位置（交叉轴对齐）
            float childY;
            switch (m_yAlignment) {
                case Y_START:
                    childY = containerY + m_padding;
                    break;
                case Y_CENTER:
                    childY = containerY + m_padding + (availableHeight - child->getHeight()) / 2.0f;
                    break;
                case Y_END:
                    childY = containerY + m_padding + availableHeight - child->getHeight();
                    break;
                default:
                    childY = containerY + m_padding;
                    break;
            }
            
            child->setPosition(currentX, childY);
            currentX += child->getWidth() + spacing;
        }
    }
}

void FlexLayout::layoutVertical(const std::vector<std::shared_ptr<UIComponent>>& children,
                               float containerX, float containerY,
                               float containerWidth, float containerHeight) {
    float availableWidth = containerWidth - 2 * m_padding;
    float availableHeight = containerHeight - 2 * m_padding;
    
    // 计算所有子组件的总高度
    float totalChildHeight = 0;
    for (auto& child : children) {
        if (child) {
            totalChildHeight += child->getHeight();
        }
    }
    
    float totalSpacing = (children.size() - 1) * m_spacing;
    float remainingHeight = availableHeight - totalChildHeight - totalSpacing;
    
    float startY = containerY + m_padding;
    float currentSpacing = m_spacing;
    
    // 根据 Y 轴对齐方式调整起始位置和间距（主轴分布）
    switch (m_yAlignment) {
        case Y_START:
            // 默认顶对齐，不需要调整
            break;
        case Y_CENTER:
            startY += remainingHeight / 2.0f;
            break;
        case Y_END:
            startY += remainingHeight;
            break;
        case Y_SPACE_BETWEEN:
            if (children.size() > 1) {
                currentSpacing = remainingHeight / (children.size() - 1);
            }
            break;
        case Y_SPACE_AROUND:
            if (children.size() > 0) {
                float extraSpace = remainingHeight / children.size();
                startY += extraSpace / 2.0f;
                currentSpacing += extraSpace;
            }
            break;
    }
    
    // 布局所有组件
    float currentY = startY;
    for (auto& child : children) {
        if (child) {
            // 计算子组件在交叉轴（X轴）上的位置
            float childX = calculateXPosition(containerX + m_padding, availableWidth, child->getWidth());
            
            child->setPosition(childX, currentY);
            currentY += child->getHeight() + currentSpacing;
        }
    }
}

float FlexLayout::calculateXPosition(float containerX, float containerWidth, float childWidth) {
    switch (m_xAlignment) {
        case X_START:
            return containerX;
        case X_CENTER:
            return containerX + (containerWidth - childWidth) / 2.0f;
        case X_END:
            return containerX + containerWidth - childWidth;
        default:
            return containerX;
    }
}

float FlexLayout::calculateYPosition(float containerY, float containerHeight, float childHeight) {
    switch (m_yAlignment) {  // 修正：使用正确的 Y 轴对齐参数
        case Y_START:
            return containerY;
        case Y_CENTER:
            return containerY + (containerHeight - childHeight) / 2.0f;
        case Y_END:
            return containerY + containerHeight - childHeight;
        default:
            return containerY;
    }
}