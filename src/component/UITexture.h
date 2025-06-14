#pragma once
#include "UIComponent.h"
#include <string>

/**
 * @class UITexture
 * @brief 纹理/图像控件类
 * @description 用于显示图像文件的UI组件，支持PNG、JPEG、BMP等格式
 */
class UITexture : public UIComponent {
public:
    // 构造函数
    UITexture(float x, float y, float width, float height, const std::string& imagePath = "");
    virtual ~UITexture();
    
    // 重写基类虚函数
    void render(NVGcontext* vg) override;
    void update(double deltaTime) override;
    bool handleEvent(const UIEvent& event) override;
    
    // 纹理特有接口
    bool loadImage(NVGcontext* vg, const std::string& imagePath);
    void unloadImage(NVGcontext* vg);
    
    void setImagePath(const std::string& imagePath);
    const std::string& getImagePath() const { return m_imagePath; }
    float getOriginWidth() {return m_OriginWidth;}
    void setOriginWidth(float OriginWidth);
    // 显示模式
    enum class ScaleMode {
        STRETCH,        // 拉伸填充
        KEEP_ASPECT,    // 保持宽高比
        ORIGINAL_SIZE   // 原始尺寸
    };
    
    void setScaleMode(ScaleMode mode) { m_scaleMode = mode; }
    ScaleMode getScaleMode() const { return m_scaleMode; }
    
    // 透明度
    void setAlpha(float alpha) { m_alpha = alpha; }
    float getAlpha() const { return m_alpha; }
    
    // 图像信息
    int getImageWidth() const { return m_imageWidth; }
    int getImageHeight() const { return m_imageHeight; }
    bool isImageLoaded() const { return m_nvgImage != -1; }
    // 添加带NVGcontext的版本，可以立即释放资源
    void setImagePath(NVGcontext* vg, const std::string& imagePath);
private:
    std::string m_imagePath;
    int m_nvgImage;          // NanoVG 图像句柄
    int m_imageWidth;
    int m_imageHeight;
    ScaleMode m_scaleMode;
    float m_alpha;
    bool m_needsLoad;        // 添加标志位
    float m_OriginWidth =600;   //添加默认值，防止窗口切换图片导致texture控件无限缩小
    // 计算渲染区域
    void calculateRenderBounds(float& renderX, float& renderY, 
                              float& renderW, float& renderH) const;
};


