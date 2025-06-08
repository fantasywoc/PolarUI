#include "UIAnimationManager.h"
#include "../component/UIComponent.h"
#include <algorithm>
#include <iostream>

UIAnimationManager& UIAnimationManager::getInstance() {
    static UIAnimationManager instance;
    return instance;
}

void UIAnimationManager::addAnimation(std::shared_ptr<UIAnimation> animation, UIComponent* target) {
    if (!animation || !target) {
        return;
    }
    
    m_animations.emplace_back(animation, target);
    animation->start();
}

void UIAnimationManager::removeAnimation(UIComponent* target) {
    if (!target) {
        return;
    }
    
    m_animations.erase(
        std::remove_if(m_animations.begin(), m_animations.end(),
            [target](const AnimationInfo& info) {
                return info.target == target;
            }),
        m_animations.end()
    );
}

void UIAnimationManager::removeAllAnimations() {
    m_animations.clear();
}

void UIAnimationManager::update(double deltaTime) {
    // 更新所有动画
    for (auto& info : m_animations) {
        if (info.isActive && info.animation) {
            info.animation->update(deltaTime);
            
            // 检查动画是否完成
            if (info.animation->isFinished()) {
                info.isActive = false;
            }
        }
    }
    
    // 移除已完成的动画
    m_animations.erase(
        std::remove_if(m_animations.begin(), m_animations.end(),
            [](const AnimationInfo& info) {
                return !info.isActive;
            }),
        m_animations.end()
    );
}

void UIAnimationManager::fadeIn(UIComponent* target, float duration, UIAnimation::EasingType easing) {
    if (!target) return;
    
    auto animation = std::make_shared<UIAnimation>(UIAnimation::FADE, duration, easing);
    animation->setValues(target->getAnimationOpacity(), 1.0f);
    animation->setOnUpdate([target](float value) {
        target->setAnimationOpacity(value);
    });
    
    addAnimation(animation, target);
}

void UIAnimationManager::fadeOut(UIComponent* target, float duration, UIAnimation::EasingType easing) {
    if (!target) return;
    
    auto animation = std::make_shared<UIAnimation>(UIAnimation::FADE, duration, easing);
    animation->setValues(target->getAnimationOpacity(), 0.0f);
    animation->setOnUpdate([target](float value) {
        target->setAnimationOpacity(value);
    });
    
    addAnimation(animation, target);
}

void UIAnimationManager::moveTo(UIComponent* target, float x, float y, float duration, UIAnimation::EasingType easing) {
    if (!target) return;
    
    float startX = target->getX() + target->getAnimationOffsetX();
    float startY = target->getY() + target->getAnimationOffsetY();
    
    auto animation = std::make_shared<UIAnimation>(UIAnimation::MOVE, duration, easing);
    animation->setValues(0.0f, 1.0f);
    animation->setOnUpdate([target, startX, startY, x, y](float progress) {
        float currentX = startX + (x - startX) * progress;
        float currentY = startY + (y - startY) * progress;
        target->setAnimationOffsetX(currentX - target->getX());
        target->setAnimationOffsetY(currentY - target->getY());
    });
    
    addAnimation(animation, target);
}

void UIAnimationManager::scaleTo(UIComponent* target, float scaleX, float scaleY, float duration, UIAnimation::EasingType easing, UIAnimation::ScaleOrigin origin) {
    if (!target) return;
    
    float startScaleX = target->getAnimationScaleX();
    float startScaleY = target->getAnimationScaleY();
    
    auto animation = std::make_shared<UIAnimation>(UIAnimation::SCALE, duration, easing);
    animation->setValues(0.0f, 1.0f);
    animation->setScaleOrigin(origin);
    animation->setOnUpdate([target, startScaleX, startScaleY, scaleX, scaleY, origin](float progress) {
        float currentScaleX = startScaleX + (scaleX - startScaleX) * progress;
        float currentScaleY = startScaleY + (scaleY - startScaleY) * progress;
        target->setAnimationScaleX(currentScaleX);
        target->setAnimationScaleY(currentScaleY);
        
        // 根据缩放原点调整位置
        if (origin == UIAnimation::CENTER) {
            float offsetX = target->getWidth() * (1.0f - currentScaleX) * 0.5f;
            float offsetY = target->getHeight() * (1.0f - currentScaleY) * 0.5f;
            target->setAnimationOffsetX(offsetX);
            target->setAnimationOffsetY(offsetY);
        }
    });
    
    addAnimation(animation, target);
}

void UIAnimationManager::rotateTo(UIComponent* target, float angle, float duration, UIAnimation::EasingType easing, UIAnimation::RotateOrigin origin) {
    if (!target) return;
    
    // 将角度转换为弧度
    angle = angle * 3.14159265359f / 180.0f;
    
    float startAngle = target->getAnimationRotation();
    
    auto animation = std::make_shared<UIAnimation>(UIAnimation::ROTATE, duration, easing);
    animation->setValues(0.0f, 1.0f);
    animation->setRotateOrigin(origin);
    animation->setOnUpdate([target, startAngle, angle](float progress) {
        float currentAngle = startAngle + (angle - startAngle) * progress;
        target->setAnimationRotation(currentAngle);
    });
    
    addAnimation(animation, target);
}

bool UIAnimationManager::hasAnimations(UIComponent* target) const {
    return std::any_of(m_animations.begin(), m_animations.end(),
        [target](const AnimationInfo& info) {
            return info.target == target && info.isActive;
        });
}

size_t UIAnimationManager::getAnimationCount() const {
    return std::count_if(m_animations.begin(), m_animations.end(),
        [](const AnimationInfo& info) {
            return info.isActive;
        });
}