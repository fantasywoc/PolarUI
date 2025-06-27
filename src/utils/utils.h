#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <cstdlib>
#include "../component/stb_image.h" // 需先下载stb_image.h
#include <filesystem>
namespace fs = std::filesystem;


// 检查是否是支持的图像文件
static const std::set<std::string> imageExtensions = {
    ".jpg", ".jpeg", ".png", ".bmp", ".gif", ".tiff", ".webp"
};


bool getImageInfo(const std::string& filePath, int& w, int& h) {
    int channels;
    // 检查文件是否存在
    if (!fs::exists(filePath)) {
        std::cerr << "文件不存在: " << filePath << std::endl;
        return false;
    }

    // 打开文件并定位到末尾
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filePath << std::endl;
        return false;
    }

    // 检查文件大小是否有效
    const long fileSize = file.tellg();
    if (fileSize <= 0) {
        std::cerr << "无效的文件大小: " << fileSize << std::endl;
        file.close();
        return false;
    }

    // 动态计算头部大小（上限2KB）
    // 修改headerSize的计算逻辑
    const size_t headerSize = (fileSize > 51768) ? 51768 : static_cast<size_t>(fileSize); // 32KB覆盖99%的EXIF偏移
    file.seekg(0, std::ios::beg);

    // 读取头部数据到vector（自动内存管理）
    std::vector<unsigned char> header(headerSize);
    if (!file.read(reinterpret_cast<char*>(header.data()), headerSize)) {
        std::cerr << "读取文件头失败: " << filePath << std::endl;
        file.close();
        return false;
    }
    file.close();

    // 解析图像元数据
    if (stbi_info_from_memory(header.data(), headerSize, &w, &h, &channels) == 0) {
        std::cerr << "不支持的图像格式或损坏的文件: " << filePath << std::endl;
        return false;
    }
    return true;
}



// 查找指定目录下的图片文件路径和文件名
void find_image_files(
    const fs::path& directory, 
    std::vector<fs::path>& image_paths,
    std::vector<std::string>& image_names
) {
    // 清空结果容器确保每次调用都是全新结果
    image_paths.clear();
    image_names.clear();

    // 验证目录有效性 
    if (!fs::exists(directory)) {
        std::cerr << "错误：路径不存在 - " << directory << std::endl;
        return;
    }
    if (!fs::is_directory(directory)) {
        std::cerr << "错误：目标不是目录 - " << directory << std::endl;
        return;
    }

    try {
        // 创建递归迭代器
        auto options = fs::directory_options::skip_permission_denied;
        fs::recursive_directory_iterator dir_iter(directory, options);
        fs::recursive_directory_iterator end_iter;

        while (dir_iter != end_iter) {
            try {
                const auto& entry = *dir_iter;
                
                // 跳过非常规文件（目录/符号链接等）
                if (entry.is_regular_file()) {
                    // 获取小写扩展名以统一比较
                    std::string ext = entry.path().extension().string();
                    std::transform(ext.begin(), ext.end(), ext.begin(), 
                                  [](unsigned char c) { return std::tolower(c); });
                    
                    // 检查是否为图片格式 
                    if (imageExtensions.find(ext) != imageExtensions.end()) {
                        image_paths.push_back(entry.path());
                        image_names.push_back(entry.path().filename().string());
                    }
                }
                ++dir_iter;
            } 
            // 捕获单个文件处理中的异常（不影响整体遍历）
            catch (const fs::filesystem_error& e) {
                std::cerr << "跳过无法访问的文件: " << e.what() << std::endl;
                dir_iter.disable_recursion_pending();  // 继续遍历其他文件 [7](@ref)
                ++dir_iter;
            }
        }
    } 
    // 捕获全局性异常（如目录打开失败）
    catch (const fs::filesystem_error& e) {
        std::cerr << "文件系统错误: " << e.what() << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "未知错误: " << e.what() << std::endl;
    }
}

size_t findPathIndex(const std::vector<fs::path>& paths, const fs::path& target) 
{
    auto it = std::find(paths.begin(), paths.end(), target);
    if (it != paths.end()) {
        return std::distance(paths.begin(), it); // 返回索引
    }
    return static_cast<size_t>(-1); // 未找到时返回无效索引
}




void removeZero(std::string& str) {
    if (str.empty()) return;

    // 使用反向迭代器从末尾开始扫描
    auto it = str.rbegin();
    while (it != str.rend()) {
        if (*it == '0') {
            // 删除当前字符（将反向迭代器转为正向迭代器）
            str.erase(std::next(it).base());
            // 重置迭代器（因删除后原迭代器失效）
            it = str.rbegin();
        } else if (*it == '.') {
            // 删除小数点并终止循环
            str.erase(std::next(it).base());
            break;
        } else {
            // 遇到非0非.字符，终止处理
            break;
        }
    }
}

//格式化曝光时间
std::string fomatExposureTime(double& exposureTime) {
    std::string str;
    if (exposureTime < 1.0f) {
        exposureTime = 1/ exposureTime;
        str = std::to_string(exposureTime);
        removeZero(str);
    }else{
        str = std::to_string(exposureTime);
        removeZero(str);
        str += "s";
    }
    return str;
}


bool isFile(const std::string& path) {
    try {
        return fs::is_regular_file(path);
    } catch (...) {
        return false;
    }
}

bool isDirectory(const std::string& path) {
    try {
        return fs::is_directory(path);
    } catch (...) {
        return false;
    }
}

std::string getDirectoryFromPath(const std::string& path) {
    try {
        if (fs::is_directory(path)) {
            return path;
        }

        std::string ext = fs::path(path).extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        
        if (imageExtensions.count(ext) > 0) {
            return fs::path(path).parent_path().string();
        }
        
        return path; // 如果不是图像文件，返回原路径
    } catch (...) {
        return path; // 异常情况下返回原路径
    }
}