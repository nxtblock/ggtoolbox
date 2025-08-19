#ifndef NBS_H
#define NBS_H
#include <raylib.h>
#include <bits/stdc++.h>
#include "nbsfont.h"
/// 缓动函数：正弦出缓动
/// @param t 时间参数（0 到 1）
/// @return 缓动值
float ESineO(float t) {
    return sin(t * PI / 2);
}

// 工具函数和宏
#define CLAMP(v,mi,ma) ((v)<(mi)?(mi):((v)>(ma)?(ma):(v)))

using namespace std;
/// 绘制带 Mica 效果的矩形
/// @param x 矩形左上角的 X 坐标
/// @param y 矩形左上角的 Y 坐标
/// @param width 矩形宽度
/// @param height 矩形高度
/// @param roundness 圆角半径
/// @param color 矩形颜色
void DrawMicaRectangle(float x, float y, float width, float height, float roundness, Color color) {
    // 绘制主矩形（半透明圆角）
    const Rectangle mainRect = {x, y, width, height};
    DrawRectangleRounded(mainRect, roundness, 8, color);
}

/// 绘制输入框组件
/// @param p 输入框位置
/// @param w 输入框宽度
/// @param fs 字号
/// @param s 内边距
/// @param c 输入框颜色
/// @param f 是否启用输入功能（1 启用，0 禁用）
/// @return 返回输入框中的文本
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

/// 绘制普通按钮
/// @param p 按钮位置
/// @param w 按钮宽度
/// @param h 按钮高度
/// @param t 按钮文本
/// @param c 按钮颜色
/// @param curvature 按钮圆角程度
/// @return 如果按钮被点击，返回 true；否则返回 false
bool DrawMicaButton(Vector2 p, float w, float h, string t, Color c, float curvature, int fs = 30) {
    Rectangle r = {p.x, p.y, w, h};
    bool isHover = CheckCollisionPointRec(GetMousePosition(), r);
    bool isClick = isHover && IsMouseButtonDown(0);

    Vector2 np = {p.x, p.y};

    if (isHover) {
        DrawMicaRectangle(np.x, np.y, w, h, curvature, c);
        DrawMicaRectangle(np.x, np.y, w, h, curvature, Fade(WHITE, 0.2));
    } else {
        DrawMicaRectangle(np.x, np.y, w, h, curvature, c);
    }
    DrawTextUTF(t, {np.x + w / 2 - MeasureTextEx(GetDynamicFont(t.c_str(), fs), t.c_str(), fs, 0).x / 2, np.y + h / 2 - fs / 2}, fs, 0, WHITE);

    return isClick;
}

/// 绘制带图标的按钮
/// @param texture 图标纹理
/// @param position 按钮位置
/// @param width 按钮长
/// @param height 按钮宽
/// @param text 按钮颜色
/// @param color 按钮颜色
/// @param curvature 按钮圆角程度
bool DrawMicaImageButton(Texture2D texture, Vector2 position, float width, float height, string text, Color color, float curvature) {
    bool check = DrawMicaButton({position.x, position.y}, width, height, text, color, curvature);

    float scale = fminf(width / texture.width, height / texture.height);
    scale = fminf(scale, 1.0f) * 0.8;

    float scaledWidth = texture.width * scale;
    float scaledHeight = texture.height * scale;

    Vector2 imagePosition = {
        position.x + 5,
        position.y + (height - scaledHeight) / 2
    };

    DrawTextureEx(texture, imagePosition, 0.0f, scale, WHITE);
    return check;
}
bool DrawMcButton(Vector2 p, float w, float h, const char *t, Color c, int fs) {
    Rectangle r = {p.x, p.y, w, h};
    bool isHover = CheckCollisionPointRec(GetMousePosition(), r);
    bool isClick = isHover && IsMouseButtonDown(0);

    // 按下时缩小
    float scale = isClick ? 0.9f : 1.0f;
    float scaledW = w * scale;
    float scaledH = h * scale;
    Vector2 scaledP = {p.x + (w - scaledW) / 2, p.y + (h - scaledH) / 2};

    // 主按钮
    DrawMicaButton(scaledP, scaledW, scaledH, t, c, 0, fs);

    // 高光和暗部
    Color lighter = {
        (unsigned char)CLAMP(c.r + 30, 0, 255),
        (unsigned char)CLAMP(c.g + 30, 0, 255),
        (unsigned char)CLAMP(c.b + 30, 0, 255),
        c.a
    };
    DrawRectangleV({scaledP.x, scaledP.y - 3}, {scaledW, 3}, lighter);

    Color darker = {
        (unsigned char)CLAMP(c.r - 15, 0, 255),
        (unsigned char)CLAMP(c.g - 15, 0, 255),
        (unsigned char)CLAMP(c.b - 15, 0, 255),
        c.a
    };
    DrawRectangleV({scaledP.x - 3, scaledP.y}, {3, scaledH}, darker);
    DrawRectangleV({scaledP.x + scaledW, scaledP.y}, {3, scaledH}, darker);

    darker = {
        (unsigned char)CLAMP(c.r - 30, 0, 255),
        (unsigned char)CLAMP(c.g - 30, 0, 255),
        (unsigned char)CLAMP(c.b - 30, 0, 255),
        c.a
    };
    DrawRectangleV({scaledP.x, scaledP.y + scaledH}, {scaledW, 3}, darker);

    return isClick;
}
bool DrawMcImageButton(Texture2D texture, Vector2 position, float width, float height, string text, Color color,int siz=28) {
    bool check = DrawMcButton({position.x, position.y}, width, height, text.c_str(), color,siz);

    float scale = fminf(width / texture.width, height / texture.height);
    scale = fminf(scale, 1.0f) * 0.7;

    float scaledWidth = texture.width * scale;
    float scaledHeight = texture.height * scale;

    Vector2 imagePosition = {
        position.x + 5 + 10 * check,
        position.y + (height - scaledHeight) / 2
    };

    DrawTextureEx(texture, imagePosition, 0.0f, scale, WHITE);
    return check;
}

/// 绘制带 Minecraft 风格高光和暗部的矩形
/// @param x 矩形左上角的 X 坐标
/// @param y 矩形左上角的 Y 坐标
/// @param width 矩形宽度
/// @param height 矩形高度
/// @param roundness 圆角半径（保留参数，实际不处理圆角）
/// @param color 矩形颜色
void DrawMcRectangle(float x, float y, float width, float height, Color color) {
    // 主矩形
    DrawMicaRectangle(x, y, width, height, 0, color);

    // 高光
    Color lighter = {
        (unsigned char)CLAMP(color.r + 30, 0, 255),
        (unsigned char)CLAMP(color.g + 30, 0, 255),
        (unsigned char)CLAMP(color.b + 30, 0, 255),
        color.a
    };
    DrawRectangleV({x, y - 3}, {width, 3}, lighter);

    // 暗部
    Color darker = {
        (unsigned char)CLAMP(color.r - 15, 0, 255),
        (unsigned char)CLAMP(color.g - 15, 0, 255),
        (unsigned char)CLAMP(color.b - 15, 0, 255),
        color.a
    };
    DrawRectangleV({x - 3, y}, {3, height}, darker);
    DrawRectangleV({x + width, y}, {3, height}, darker);

    darker = {
        (unsigned char)CLAMP(color.r - 30, 0, 255),
        (unsigned char)CLAMP(color.g - 30, 0, 255),
        (unsigned char)CLAMP(color.b - 30, 0, 255),
        color.a
    };
    DrawRectangleV({x, y + height}, {width, 3}, darker);

  
}
string DrawMcInputbox(Vector2 p, float w, int fs, float s, Color c, int f) {
    static string t;
    static bool a = false;
    float h = fs * 1.4f;
    Rectangle r = {p.x, p.y, w, h};

    // 动画逻辑
    static float an = 0;
    an += (a ? 8 : -8) * GetFrameTime();
    an = CLAMP(an, 0, 5);

    // 使用 DrawMcRectangle 绘制输入框背景
    DrawMcRectangle(p.x, p.y, w, h, c);

    // 检测点击区域
    if (f && CheckCollisionPointRec(GetMousePosition(), r)) {
        if (IsMouseButtonPressed(0)) a = true;
    } else if (IsMouseButtonPressed(0)) {
        a = false;
    }

    // 输入处理
    if (a && f) {
        int k = GetCharPressed();
        while (k > 0) {
            t += (char)k;
            k = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (!t.empty()) t = t.substr(0, t.size() - 1);
        }
    }

    // 绘制文本 + 光标闪烁
    if (f == 1) {
        string show = t;
        if ((int)(GetTime()) % 2 == 0 && !t.empty()) {
            DrawTextUTF(t.c_str(), {p.x + s, p.y + (h - fs) / 2}, fs, 0, WHITE);
        } else {
            show += "|";
            DrawTextUTF(show.c_str(), {p.x + s, p.y + (h - fs) / 2}, fs, 0, WHITE);
        }
    }


    return t;
}
#endif //NBS_H