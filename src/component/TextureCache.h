#pragma once

#include <string>
#include <functional>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <unordered_map>
#include <nanovg.h>

// 避免Windows API冲突，先包含utils.h
#include "../utils/utils.h"

#if defined(_WIN32)
    #include <windows.h>
    // 取消Windows API的LoadImage宏定义
    #ifdef LoadImage
        #undef LoadImage
    #endif
#elif defined(__APPLE__) || defined(__unix__) 
    #include <unistd.h>
    #include <cstdint>
#endif

class TextureCache {
public:
    struct TextureInfo {
        int nvgHandle = -1;
        int refCount = 0;
        bool loading = false;
    };

    // 添加引用计数管理
    static void addRef(const std::string& path) {
        std::lock_guard<std::mutex> lock(s_mutex);
        auto it = s_textureCache.find(path);
        if (it != s_textureCache.end()) {
            it->second.refCount++;
        } else {
            s_textureCache[path] = TextureInfo{-1, 1, false};
        }
    }

    static void removeRef(const std::string& path, NVGcontext* vg) {
        std::lock_guard<std::mutex> lock(s_mutex);
        auto it = s_textureCache.find(path);
        if (it != s_textureCache.end()) {
            it->second.refCount--;
            if (it->second.refCount <= 0) {
                if (it->second.nvgHandle != -1) {
                    nvgDeleteImage(vg, it->second.nvgHandle);
                }
                s_textureCache.erase(it);
            }
        }
    }

    static void preloadTexture(NVGcontext* vg, const std::string& path) {
        {
            std::lock_guard<std::mutex> lock(s_mutex);
            auto it = s_textureCache.find(path);
            if (it != s_textureCache.end()) {
                if (!it->second.loading) return;
            } else {
                s_textureCache[path] = TextureInfo{-1, 0, true};
            }
        }
        
        // 在后台线程加载
        std::thread([vg, path]() {
            int width, height, channels;
            // 使用完全限定名称调用utils.h中的LoadImage
            unsigned char* data = ::LoadImage(path.c_str(), width, height, channels);
            if (!data) return;
            
            // 将创建纹理的任务提交到主线程队列
            postToMainThread([vg, path, data, width, height]() {
                int nvgImage = nvgCreateImageRGBA(vg, width, height, 0, data);
                {
                    std::lock_guard<std::mutex> lock(s_mutex);
                    auto it = s_textureCache.find(path);
                    if (it != s_textureCache.end()) {
                        it->second.nvgHandle = nvgImage;
                        it->second.loading = false;
                    }
                }
                // 修复FreeImage调用
                unsigned char* dataPtr = data;
                FreeImage(dataPtr, path);
            });
        }).detach();
    }
    
    static int getTexture(const std::string& path) {
        std::lock_guard<std::mutex> lock(s_mutex);
        auto it = s_textureCache.find(path);
        return (it != s_textureCache.end()) ? it->second.nvgHandle : -1;
    }
    
    static void cleanup(NVGcontext* vg) {
        std::lock_guard<std::mutex> lock(s_mutex);
        for (const auto& pair : s_textureCache) {
            if (pair.second.nvgHandle != -1) {
                nvgDeleteImage(vg, pair.second.nvgHandle);
            }
        }
        s_textureCache.clear();
    }

    static void processMainThreadTasks() {
        std::queue<std::function<void()>> tasksToExecute;
        {
            std::lock_guard<std::mutex> lock(s_mutex);
            tasksToExecute = std::move(s_mainThreadTasks);
            s_mainThreadTasks = std::queue<std::function<void()>>();
        }
        
        while (!tasksToExecute.empty()) {
            auto task = std::move(tasksToExecute.front());
            tasksToExecute.pop();
            task();
        }
    }

private:
    static void postToMainThread(std::function<void()> task) {
        std::lock_guard<std::mutex> lock(s_mutex);
        s_mainThreadTasks.push(std::move(task));
    }

    static std::mutex s_mutex;
    static std::unordered_map<std::string, TextureInfo> s_textureCache;
    static std::queue<std::function<void()>> s_mainThreadTasks;
};