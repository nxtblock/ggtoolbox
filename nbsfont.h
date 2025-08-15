#ifndef NBSFONT_H
#define NBSFONT_H

#include "raylib.h"
#include <string>
#include <set>
using namespace std;

/// 用于缓存已加载字体的结构体
struct CachedFont {
    string key; // 唯一标识（例如文本＋字号）
    Font fnt; // 字体对象

    bool operator<(const CachedFont &other) const {
        return key < other.key;
    } 
};

// 字体缓存和字体数据
static set<CachedFont> fntCache;
static int fntFileSize = 0;
static unsigned char *fntFileData = nullptr;

/// 初始化字体系统（程序启动时调用）
bool InitFontSystem(const char *fontPath) {
    fntFileData = LoadFileData(fontPath, &fntFileSize);
    return (fntFileData != nullptr && fntFileSize > 0);
}

/// 卸载字体系统（程序结束时调用）
void UnloadFontSystem() {
    for (const auto& item : fntCache) {
        UnloadFont(item.fnt);
    }
    fntCache.clear();

    if (fntFileData) {
        UnloadFileData(fntFileData);
        fntFileData = nullptr;
        fntFileSize = 0;
    }
}

/// 动态加载字体的函数
Font GetDynamicFont(const char *txt, int fntSize = 32) {
    string key = string(txt) + "_" + to_string(fntSize);
    CachedFont dummy{key, {}};
    auto it = fntCache.find(dummy);
    if (it != fntCache.end()) {
        return it->fnt;
    }

    int cpCount = 0;
    int *cps = LoadCodepoints(txt, &cpCount);
    Font fnt = LoadFontFromMemory(".ttf", fntFileData, fntFileSize, fntSize, cps, cpCount);
    UnloadCodepoints(cps);

    fntCache.insert({key, fnt});
    return fnt;
}

/// 绘制 UTF-8 文本
void DrawTextUTF(string stxt, Vector2 pos, int fntSize, float spacing, Color color) {
    if (fntSize==0 or stxt.empty()) return;
    if(stxt.size()%2)stxt+="        ";
    else stxt+="       ";
    Font fnt = GetDynamicFont(stxt.c_str(), fntSize);
    DrawTextEx(fnt, stxt.c_str(), pos, (float)fntSize, spacing, color);
}

#endif // NBSFONT_H
