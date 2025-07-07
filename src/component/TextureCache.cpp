#include "TextureCache.h"

std::mutex TextureCache::s_mutex;
std::unordered_map<std::string, TextureCache::TextureInfo> TextureCache::s_textureCache;
std::queue<std::function<void()>> TextureCache::s_mainThreadTasks;