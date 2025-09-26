#include "raylib.h"
#include <map>
#include <string>
#include <set>
#include <vector>
#include <iostream>

// ======================
// 字体系统封装
// ======================
static std::string g_fontPath;
static std::map<int, Font> g_fontCache;
static std::map<int, std::set<int>> g_fontGlyphs; // 每个字号存储已加载的码点集合
static const int MAX_RETRY = 3;

// UTF-8 -> Unicode 码点解码
static std::vector<int> Utf8ToCodepoints(const std::string &utf8) {
    std::vector<int> cps;
    const unsigned char *s = (const unsigned char *)utf8.c_str();
    while (*s) {
        int cp = 0, bytes = 0;
        if (*s < 0x80) { cp = *s; bytes = 1; }
        else if ((*s & 0xE0) == 0xC0) { cp = *s & 0x1F; bytes = 2; }
        else if ((*s & 0xF0) == 0xE0) { cp = *s & 0x0F; bytes = 3; }
        else if ((*s & 0xF8) == 0xF0) { cp = *s & 0x07; bytes = 4; }
        else { s++; continue; }

        for (int i = 1; i < bytes; i++) {
            if ((s[i] & 0xC0) == 0x80)
                cp = (cp << 6) | (s[i] & 0x3F);
        }
        cps.push_back(cp);
        s += bytes;
    }
    return cps;
}

// 初始化字体系统
void InitFontSystem(const char *fontPath) {
    g_fontPath = fontPath;
    g_fontCache.clear();
    g_fontGlyphs.clear();
}

// 内部加载函数（带重试）
static Font LoadFontWithRetry(const char *path, int size, const std::vector<int> &codepoints) {
    Font font = {0};
    int count = codepoints.size();
    std::vector<int> glyphs = codepoints;
    if (glyphs.empty()) {
        // 至少加载 ASCII，避免空崩溃
        for (int i = 32; i < 128; i++) glyphs.push_back(i);
    }

    for (int attempt = 1; attempt <= MAX_RETRY; attempt++) {
        font = LoadFontEx(path, size, glyphs.data(), count);
        if (font.texture.id != 0) {
            return font;
        }
        std::cerr << "[FontSystem] Load attempt " << attempt
                  << " failed for size " << size << std::endl;
    }
    std::cerr << "[FontSystem] Failed to load font after retries, using default" << std::endl;
    return GetFontDefault();
}

// 获取指定大小的字体（会按需加载新字形）
Font GetDynamicFont(const char *txt, int fntSize = 32) {
    std::vector<int> cps = Utf8ToCodepoints(txt);

    // 检查是否已有缓存
    if (!g_fontCache.count(fntSize)) {
        // 首次加载（只含 ASCII）
        std::vector<int> initGlyphs;
        for (int i = 32; i < 128; i++) initGlyphs.push_back(i);
        g_fontCache[fntSize] = LoadFontWithRetry(g_fontPath.c_str(), fntSize, initGlyphs);
        g_fontGlyphs[fntSize].insert(initGlyphs.begin(), initGlyphs.end());
    }

    // 检查需要补充的字形
    std::set<int> &loadedGlyphs = g_fontGlyphs[fntSize];
    std::vector<int> newGlyphs;
    for (int cp : cps) {
        if (!loadedGlyphs.count(cp)) {
            newGlyphs.push_back(cp);
            loadedGlyphs.insert(cp);
        }
    }

    // 如果有新字形，重新加载字体并覆盖
    if (!newGlyphs.empty()) {
        std::vector<int> allGlyphs(loadedGlyphs.begin(), loadedGlyphs.end());
        g_fontCache[fntSize] = LoadFontWithRetry(g_fontPath.c_str(), fntSize, allGlyphs);
    }

    return g_fontCache[fntSize];
}

// 绘制 UTF-8 文本
void DrawTextUTF(const std::string &stxt, Vector2 pos, int fntSize, float spacing, Color color) {
    Font font = GetDynamicFont(stxt.c_str(), fntSize);
    DrawTextEx(font, stxt.c_str(), pos, (float)fntSize, spacing, color);
}

// 卸载字体系统
void UnloadFontSystem() {
    for (auto &it : g_fontCache) {
        if (it.second.texture.id != 0) {
            UnloadFont(it.second);
        }
    }
    g_fontCache.clear();
    g_fontGlyphs.clear();
    g_fontPath.clear();
}
