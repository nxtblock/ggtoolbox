#ifndef NBS_H
#define NBS_H
#include <raylib.h>
#include <bits/stdc++.h>
// 工具函数和宏
#define CLAMP(v,mi,ma) ((v)<(mi)?(mi):((v)>(ma)?(ma):(v)))

using namespace std;

// 用于缓存已加载字体的结构体
struct CachedFont {
    string key; // 唯一标识（例如文本＋字号）
    Font fnt;
    // 重载 < 运算符，便于 set 内部排序
    bool operator<(const CachedFont &other) const {
        return key < other.key;
    }
};

// 全局缓存容器
static set<CachedFont> fntCache;
// 保存字体文件数据（程序启动时加载一次）
static int fntFileSize = 0; // 修改为 int 类型
static unsigned char *fntFileData = LoadFileData("../src/NotoSansSC.ttf", &fntFileSize);

// 动态加载字体的函数：传入 UTF-8 文本和字号，返回对应字体
Font GetDynamicFont(const char *txt, int fntSize = 32) {
    // 使用文本和字号生成唯一 key
    string key = string(txt) + "_" + to_string(fntSize);

    // 在缓存中查找是否已经加载
    CachedFont dummy{key, {}};
    auto it = fntCache.find(dummy);
    if (it != fntCache.end()) {
        return it->fnt;
    }

    // 未找到则加载字体：先获取文本的 Unicode 码点表
    int cpCount = 0;
    int *cps = LoadCodepoints(txt, &cpCount);

    // 从内存加载字体
    Font fnt = LoadFontFromMemory(".ttf", fntFileData, fntFileSize, fntSize, cps, cpCount);

    // 释放码点表内存
    UnloadCodepoints(cps);

    // 插入到缓存中
    fntCache.insert({key, fnt});

    return fnt;
}

// DrawTextUTF 函数：封装了动态加载字体并绘制 UTF-8 文本的过程
void DrawTextUTF(string stxt, Vector2 pos, int fntSize, float spc, Color clr) {
    if (stxt.empty()) return;
    stxt+="     ";
    const char *txt=stxt.c_str();
    Font fnt = GetDynamicFont(txt, fntSize);
    DrawTextEx(fnt, txt, pos, fntSize, spc, clr);
}

// DrawMicaRectangle 函数：封装了Mica效果的矩形绘制
void DrawMicaRectangle(float x, float y, float width, float height, float roundness, Color color) {
    // 绘制阴影效果
    const int sl = 5;
    const float sox = 1.0f;
    const float soy = 1.0f;
    const float sss = 1.0f;
    const Color sbc = {0, 0, 0, 40}; // 基础阴影颜色（半透明黑）

    for (int i = 0; i < sl; ++i) {
        const float spread = i * sss;
        const Color shadowColor = {
            sbc.r,
            sbc.g,
            sbc.b,
            (unsigned char) (sbc.a - i * 8) // 逐层变淡
        };
        const Rectangle shadowRect = {
            x - spread / 2 + sox,
            y - spread / 2 + soy,
            width + spread,
            height + spread
        };
        DrawRectangleRounded(shadowRect, roundness, 8, shadowColor);
    }

    // 绘制主矩形（半透明圆角）
    const Rectangle mainRect = {x, y, width, height};
    DrawRectangleRounded(mainRect, roundness, 8, color);
}

// 缓动函数实现
float ESineO(float t) { return sin(t * PI / 2); }
float EElasO(float t) { return sin(-13 * (t + 1) * PI / 2) * pow(2, -10 * t) + 1; }
float EElasI(float t) { return sin(13 * t * PI / 2) * pow(2, 10 * (t - 1)); }

// 输入框组件
string DrawMicaInputbox(Vector2 p, float w, int fs, float s, Color c, int f) {
    static string t;
    static bool a = false;
    float h = fs * 1.4f;
    Rectangle r = {p.x, p.y, w, h};

    static float an = 0;
    an += (a ? 8 : -8) * GetFrameTime();
    an = CLAMP(an, 0, 5);

    DrawMicaRectangle(p.x, p.y, w, h, ESineO(an / 5) * 5, c);

    if (f && CheckCollisionPointRec(GetMousePosition(), r)) {
        if (IsMouseButtonPressed(0)) a = true;
    } else if (IsMouseButtonPressed(0)) a = false;

    if (a && f) {
        int k = GetCharPressed();
        while (k > 0) t += (char) k, k = GetCharPressed();
        if (IsKeyPressed(KEY_BACKSPACE)) t = t.substr(0, t.size() - 1);
    }
    if (f == 1) {
        if ((int) GetTime() % 2 == 0 and t.size())
            DrawTextUTF(t.c_str(), {p.x + s, p.y + (h - fs) / 2}, fs, 0,WHITE);

        else {
            string show = t + "|";
            DrawTextUTF(show.c_str(), {p.x + s, p.y + (h - fs) / 2}, fs, 0,WHITE);
        }
    } else {
    }
    float bs = h - s * 2;
    bool hb = CheckCollisionPointRec(GetMousePosition(), {p.x + w - bs - s, p.y + s, bs, bs});
    DrawMicaRectangle(p.x + w - bs - s, p.y + s, bs * (hb ? 0.9 : 1), bs * (hb ? 0.9 : 1), bs / 2, hb ? GREEN : GRAY);
    return t;
}

// 普通按钮
bool DrawMicaButton(Vector2 p, float w, float h, const char *t, Color c, float curvature) {
    static float a[2] = {0};
    Rectangle r = {p.x, p.y, w, h};
    bool hv = CheckCollisionPointRec(GetMousePosition(), r), cl = false;

    a[0] = CLAMP(a[0]+(hv?12:-12)*GetFrameTime(), 0, 1);
    a[1] = CLAMP(a[1]+((hv&&IsMouseButtonDown(0))?12:-12)*GetFrameTime(), 0, 1);

    if (hv && IsMouseButtonReleased(0)) cl = true;

    float s = 1 - EElasO(a[1]) * 0.1f;
    Vector2 np = {p.x + w * (1 - s) / 2, p.y + h * (1 - s) / 2};
    DrawMicaRectangle(np.x, np.y, w * s, h * s, curvature, ColorAlpha(c, 0.8 + a[0] * 0.2));
    DrawTextUTF(t, {np.x + w * s / 2 - MeasureText(t, 20) / 2, np.y + h * s / 2 - 10}, 20, 0,WHITE);
    return cl;
}

// 灯泡按钮
int DrawMicaLightButton(int &st, Vector2 p, float s, const char *t, Color c, float curvature) {
    static float a[2] = {0};
    Rectangle r = {p.x, p.y, s, s};
    bool hv = CheckCollisionPointRec(GetMousePosition(), r);

    if (hv && IsMouseButtonPressed(0)) st ^= 1;

    a[0] = CLAMP(a[0]+(hv?10:-10)*GetFrameTime(), 0, 1);
    a[1] = CLAMP(a[1]+((hv&&IsMouseButtonDown(0))?10:-10)*GetFrameTime(), 0, 1);

    float sc = 1 - EElasO(a[1]) * 0.2f;
    DrawMicaRectangle(p.x + s * (1 - sc) / 2, p.y + s * (1 - sc) / 2, s * sc, s * sc, curvature,
                      ColorAlpha(st ? GOLD : c, 0.8 + a[0] * 0.2));
    DrawTextUTF(t, {p.x + s / 2 - MeasureText(t, 20) / 2, p.y + s / 2 - 10}, 20, 0,WHITE);
    return st;
}

// 带图标的灯泡按钮
int DrawMicaImageLightButton(int &st, Texture2D i, Vector2 p, float s, Color c, float curvature) {
    float is = i.height * 1.2f;
    DrawMicaLightButton(st, {p.x + is, p.y}, s, "", c, curvature);
    DrawTextureEx(i, {p.x, p.y + (s - i.height) / 2}, 0, 1,WHITE);
    return st;
}

// 进度条
float DrawMicaProgress(float &tg, Vector2 p, float w, float h, float sp) {
    static float cv = 0;
    cv += (tg - cv) * GetFrameTime() * sp;
    cv = CLAMP(cv, 0, 1);
    float pw = EElasI(cv) * w;
    DrawMicaRectangle(p.x, p.y, pw, h, h / 2, ColorAlpha(BLUE, 0.8));
    return cv;
}


#endif //NBS_H
