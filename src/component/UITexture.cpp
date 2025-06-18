#include "UITexture.h"
#include <iostream>
#include <algorithm>


#define STBI_MAX_DIMENSIONS 32768  // 扩展到 32768x32768 ,默认最大支持尺寸为 ​16,777,216 像素
// 需要包含 stb_image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::vector<UITexture*> UITexture::s_instances;

UITexture::UITexture(float x, float y, float width, float height, const std::string& imagePath)
    : UIComponent(x, y, width, height)
    , m_imagePath(imagePath)
    , m_nvgImage(-1)
    , m_imageWidth(0)
    , m_imageHeight(0)
    , m_scaleMode(ScaleMode::STRETCH)
    , m_alpha(1.0f)
    , m_OriginWidth(height)
    , m_needsLoad(!imagePath.empty()) {
    // 不在构造函数中加载图像，延迟到render时加载
    s_instances.push_back(this);
}

UITexture::~UITexture() {
    auto it = std::find(s_instances.begin(), s_instances.end(), this);
    if (it != s_instances.end()) {
        s_instances.erase(it);
    }
}

void UITexture::cleanupAll(NVGcontext* vg) {
    for (auto* texture : s_instances) {
        texture->unloadImage(vg);
    }
}

void UITexture::render(NVGcontext* vg) {
    if (!m_visible) return;
    
    if (!m_visible || (m_alpha * m_animationOpacity) <= 0.0f) {  // 检查总透明度
        return;
    }

    // 如果需要加载图像且还未加载
    if (m_needsLoad && m_nvgImage == -1 && !m_imagePath.empty()) {
        loadImage(vg, m_imagePath);
        m_needsLoad = false;
    }
    if (m_nvgImage == -1) {
        return;
    }
    

    // 缩放变换（以中心为原点）#支持中心动画缩放   放在这个位置可以缩放整个texture
    if (m_animationScaleX != 1.0f || m_animationScaleY != 1.0f) {
        // 计算全局中心点（世界坐标）
        float centerX = m_x + m_width / 2.0f;
        float centerY = m_y + m_height / 2.0f;
    
        // 变换顺序：平移到全局中心 → 缩放 → 平移回原位置
        nvgTranslate(vg, centerX, centerY);
        nvgScale(vg, m_animationScaleX, m_animationScaleY);
        nvgTranslate(vg, -centerX, -centerY); 
    }
    // // 平移
    // if (m_animationOffsetX != 0.0f || m_animationOffsetY != 0.0f){

    //     float tragetX = m_x + m_animationOffsetX  ;
    //     float tragetY = m_y + m_animationOffsetY  ;

    //     nvgTranslate(vg, tragetX, tragetY);

    // }

    // 应用动画偏移
    if (m_animationOffsetX != 0.0f || m_animationOffsetY != 0.0f) {
        std::cerr << "texture 偏移 m_animationOffsetX: " << m_animationOffsetX << " m_animationOffsetY: " << m_animationOffsetY << std::endl;
        nvgTranslate(vg, m_animationOffsetX, m_animationOffsetY);
    }
  // 绘制背景（如果设置了）
  if (m_backgroundColor.a >= 0) {
    nvgBeginPath(vg);
    nvgRoundedRect(vg, m_x, m_y, m_width, m_height, m_cornerRadius);
    // nvgFillColor(vg, m_backgroundColor);
    nvgFillColor(vg, nvgRGBA(0, 0, 0, 1)); // 全透明黑色
    nvgFill(vg);
}
    




    // 计算图像渲染区域
    float renderX, renderY, renderW, renderH;
    calculateRenderBounds(renderX, renderY, renderW, renderH);
    
    // 绘制图像
    nvgSave(vg);
  

    // 设置透明度（考虑动画透明度）
    nvgGlobalAlpha(vg, m_alpha * m_animationOpacity);


    // 创建图像填充模式
    NVGpaint imgPaint = nvgImagePattern(vg, renderX, renderY, renderW, renderH, 0, m_nvgImage, 1.0f);
    
    nvgBeginPath(vg);
    nvgRoundedRect(vg, renderX, renderY, renderW, renderH, m_cornerRadius);
    nvgFillPaint(vg, imgPaint);
    nvgFill(vg);
    





    nvgRestore(vg);
    
    // 绘制边框（如果设置了）
    // if (m_borderWidth > 0 && m_borderColor.a > 0) {
    //     nvgBeginPath(vg);
    //     nvgRoundedRect(vg, m_x, m_y, m_width, m_height, m_cornerRadius);
    //     nvgStrokeColor(vg, m_borderColor);
    //     nvgStrokeWidth(vg, m_borderWidth);
    //     nvgStroke(vg);
    // }
}

void UITexture::update(double deltaTime) {
    // 纹理控件通常不需要更新逻辑
}

bool UITexture::handleEvent(const UIEvent& event) {
    if (!m_visible || !m_enabled) {
        return false;
    }
    
    bool handled = false;
    
    switch (event.type) {
        case UIEvent::MOUSE_PRESS:
            std::cout << "UITexture received mouse button: " << event.mouseButton << std::endl;
            if (event.mouseButton == 0) { // 左键点击
                // 检测双击
                if (contains(event.mouseX, event.mouseY)) {
                    // 检测双击
                    if (m_doubleClickEnabled && 
                        event.mouseButton == m_lastClickButton &&
                        (event.clickTime - m_lastClickTime) < DOUBLE_CLICK_TIME) {
                        // 双击事件
                        if (m_onDoubleClick) {
                            m_onDoubleClick(event.mouseButton);
                        }
                        handled = true;
                    } else {
                        // 只记录鼠标按下位置，不立即进入拖拽状态
                        m_mousePressed = true;
                        m_lastMouseX = event.mouseX;
                        m_lastMouseY = event.mouseY;
                        handled = true;
                    }
                    
                    // 记录点击信息用于双击检测
                    m_lastClickTime = event.clickTime;
                    m_lastClickButton = event.mouseButton;
                }
            } else if (event.mouseButton == 2 && m_middleClickEnabled) { // 中键点击
                std::cout << "Middle click detected!" << std::endl;
                if (contains(event.mouseX, event.mouseY)) {
                    std::cout << "Middle click inside texture bounds" << std::endl;
                    if (m_onMiddleClick) {
                        std::cout << "Calling middle click callback" << std::endl;
                        m_onMiddleClick(event.mouseX, event.mouseY);
                    }
                    handled = true;
                }
            }
            break;
            
        case UIEvent::MOUSE_RELEASE:
            if (event.mouseButton == 0) { // 左键释放
                if (m_isDragging) {
                    m_isDragging = false;
                }
                m_mousePressed = false;
                handled = true;
            }
            break;
            
        case UIEvent::MOUSE_MOVE:
            if (m_mousePressed && m_dragEnabled && !m_isDragging) {
                // 检查是否移动了足够的距离才开始拖拽
                float deltaX = event.mouseX - m_lastMouseX;
                float deltaY = event.mouseY - m_lastMouseY;
                float distance = sqrt(deltaX * deltaX + deltaY * deltaY);
                
                if (distance > 3.0f) { // 移动超过3像素才开始拖拽
                    m_isDragging = true;
                }
            }
            
            if (m_isDragging && m_dragEnabled) {
                float deltaX = event.mouseX - m_lastMouseX;
                float deltaY = event.mouseY - m_lastMouseY;
                
                // 更新位置
                // setPosition(m_x + deltaX, m_y + deltaY);
                
                // 调用回调函数
                if (m_onDrag) {
                    m_onDrag(deltaX, deltaY);
                }
                
                m_lastMouseX = event.mouseX;
                m_lastMouseY = event.mouseY;
                handled = true;
            }
            break;
            
        case UIEvent::MOUSE_SCROLL:
            if (contains(event.mouseX, event.mouseY)) {
                if (m_mousePressed && !m_isDragging && m_dragScrollEnabled) {
                    // 按住左键不动时的滚轮事件（点击滚动）
                    if (m_onDragScroll) {
                        m_onDragScroll(event.scrollX, event.scrollY);
                    }
                    handled = true;
                } else if (!m_mousePressed && m_scrollEnabled) {
                    // 普通滚轮事件
                    if (m_onScroll) {
                        m_onScroll(event.scrollX, event.scrollY);
                    }
                    handled = true;
                }
            }
            break;
            
        case UIEvent::MOUSE_DOUBLE_CLICK:
            if (m_doubleClickEnabled && contains(event.mouseX, event.mouseY)) {
                if (m_onDoubleClick) {
                    m_onDoubleClick(event.mouseButton);
                }
                handled = true;
            }
            break;
            
        case UIEvent::KEY_PRESS:
            if (m_keyEventsEnabled && contains(m_lastMouseX, m_lastMouseY)) {
                // 处理特定按键
                switch (event.keyCode) {
                    case 262: // 右箭头键
                        setPosition(m_x + 10, m_y);
                        handled = true;
                        break;
                    case 263: // 左箭头键
                        setPosition(m_x - 10, m_y);
                        handled = true;
                        break;
                    case 264: // 下箭头键
                        setPosition(m_x, m_y + 10);
                        handled = true;
                        break;
                    case 265: // 上箭头键
                        setPosition(m_x, m_y - 10);
                        handled = true;
                        break;
                    case 82:  // R键 - 重置位置
                        setPosition(0, 0);
                        handled = true;
                        break;
                    case 70:  // F键 - 适应窗口
                        setScaleMode(ScaleMode::KEEP_ASPECT);
                        handled = true;
                        break;
                    case 79:  // O键 - 原始尺寸
                        setScaleMode(ScaleMode::ORIGINAL_SIZE);
                        handled = true;
                        break;
                }
                
                // 调用按键回调
                if (m_onKeyPress) {
                    m_onKeyPress(event.keyCode, event.modifiers);
                }
            }
            break;
            
        default:
            break;
    }
    
    return handled;
}

bool UITexture::loadImage(NVGcontext* vg, const std::string& imagePath) {

    

    if (!vg) {
        std::cerr << "NVGcontext is null, cannot load image" << std::endl;
        return false;
    }
    
    // 先卸载之前的图像
    unloadImage(vg);
    
    // 使用 stb_image 加载图像
    int channels;
    unsigned char* data = stbi_load(imagePath.c_str(), &m_imageWidth, &m_imageHeight, &channels, 4);
    
    if (!data) {
        std::cerr << "Failed to load image: " << imagePath << std::endl;
        std::cerr << "STB Error: " << stbi_failure_reason() << std::endl;
        return false;
    }
    
    // 计算图片宽高比
    float imageAspect = (float)m_imageWidth / (float)m_imageHeight;
    // 计算当前容器宽高比
    float containerAspect = getWidth() / getHeight();
    float newWidth = getOriginWidth() * imageAspect;
    setSize(newWidth, getOriginWidth());
    // if (imageAspect >=1) {
    //     // 图片比容器更宽，以宽度为准调整高度
    //     float newHeight = getOriginWidth() / imageAspect;
    //     setSize(getOriginWidth(), newHeight);
    // } else {
    //     // 图片比容器更高，以高度为准调整宽度
    //     float newWidth = getOriginWidth() * imageAspect;
    //     setSize(newWidth, getOriginWidth());
    // }
    // 创建 NanoVG 图像
    m_nvgImage = nvgCreateImageRGBA(vg, m_imageWidth, m_imageHeight, 0, data);
    
    // 释放 stb_image 分配的内存
    stbi_image_free(data);
    
    if (m_nvgImage == -1) {
        std::cerr << "Failed to create NanoVG image from: " << imagePath << std::endl;
        return false;
    }
    
    m_imagePath = imagePath;
    std::cout << "Loaded image: " << imagePath << " (" << m_imageWidth << "x" << m_imageHeight << ")" << std::endl;
    
    return true;
}

void UITexture::unloadImage(NVGcontext* vg) {
    if (m_nvgImage != -1 && vg) {
        nvgDeleteImage(vg, m_nvgImage);
        m_nvgImage = -1;
    }
    m_imageWidth = 0;
    m_imageHeight = 0;
}

void UITexture::setImagePath(NVGcontext* vg, const std::string& imagePath) {
    if (m_imagePath != imagePath) {
        // 先释放旧资源
        unloadImage(vg);
        // 设置新路径
        m_imagePath = imagePath;
        m_needsLoad = !imagePath.empty();
        loadImage(vg,m_imagePath);

    }
}

void UITexture::calculateRenderBounds(float& renderX, float& renderY, 
                                     float& renderW, float& renderH) const {
    switch (m_scaleMode) {
        case ScaleMode::STRETCH:
            renderX = m_x;
            renderY = m_y;
            renderW = m_width;
            renderH = m_height;
            break;
            
        case ScaleMode::KEEP_ASPECT: {
            float scaleX = m_width / (float)m_imageWidth;
            float scaleY = m_height / (float)m_imageHeight;
            float scale = std::min(scaleX, scaleY);
            
            renderW = m_imageWidth * scale;
            renderH = m_imageHeight * scale;
            renderX = m_x + (m_width - renderW) * 0.5f;
            renderY = m_y + (m_height - renderH) * 0.5f;
            break;
        }
        
        case ScaleMode::ORIGINAL_SIZE:
            renderW = (float)m_imageWidth;
            renderH = (float)m_imageHeight;
            renderX = m_x + (m_width - renderW) * 0.5f;
            renderY = m_y + (m_height - renderH) * 0.5f;
            break;
    }
}

void UITexture::setOriginWidth(float OriginWidth) {
    if(OriginWidth >0){
        m_OriginWidth = OriginWidth;
    }
}
