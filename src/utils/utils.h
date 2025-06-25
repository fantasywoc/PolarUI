#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include "../component/stb_image.h" // 需先下载stb_image.h
#include <filesystem>



bool getImageInfo(const std::string& filePath, int& w, int& h) {
    int channels;
    // 检查文件是否存在
    if (!std::filesystem::exists(filePath)) {
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
    const size_t headerSize = (fileSize > 2048) ? 2048 : static_cast<size_t>(fileSize);
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

