#ifndef NBS_H
#define NBS_H
#include <raylib.h>
#include <bits/stdc++.h>
// 工具函数和宏
#define CLAMP(v,mi,ma) ((v)<(mi)?(mi):((v)>(ma)?(ma):(v)))

using namespace std;

/// 用于缓存已加载字体的结构体
struct CachedFont {
    string key; // 唯一标识（例如文本＋字号）
    Font fnt; // 字体对象
    // 重载 < 运算符，便于 set 内部排序
    bool operator<(const CachedFont &other) const {
        return key < other.key;
    }
};

// 全局缓存容器
static set<CachedFont> fntCache;
// 保存字体文件数据（程序启动时加载一次）
static int fntFileSize = 0; // 修改为 int 类型
static unsigned char *fntFileData;

/// 动态加载字体的函数
/// @param txt 输入的 UTF-8 文本
/// @param fntSize 字号
/// @return 返回对应的字体对象
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

/// 绘制 UTF-8 文本的函数
/// @param stxt 输入的字符串
/// @param pos 文本绘制位置
/// @param fntSize 字号
/// @param spc 字符间距
/// @param clr 文本颜色
void DrawTextUTF(string stxt, Vector2 pos, int fntSize, float spc, Color clr) {
    if (stxt.empty()) return;
    stxt += "     ";
    const char *txt = stxt.c_str();
    Font fnt = GetDynamicFont(txt, fntSize);
    DrawTextEx(fnt, txt, pos, fntSize, spc, clr);
}

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
    if(color.a < 1.0f) {
        DrawRectangleRounded(mainRect, roundness, 8, color);
        return;
    }
    DrawRectangleRounded(mainRect, roundness, 8, Fade(color,0.35f));
}

/// 缓动函数：正弦出缓动
/// @param t 时间参数（0 到 1）
/// @return 缓动值
float ESineO(float t) { return sin(t * PI / 2); }

/// 缓动函数：弹性出缓动
/// @param t 时间参数（0 到 1）
/// @return 缓动值
float EElasO(float t) { return sin(-13 * (t + 1) * PI / 2) * pow(2, -10 * t) + 1; }

/// 缓动函数：弹性入缓动
/// @param t 时间参数（0 到 1）
/// @return 缓动值
float EElasI(float t) { return sin(13 * t * PI / 2) * pow(2, 10 * (t - 1)); }

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
bool DrawMicaButton(Vector2 p, float w, float h, string t, Color c, float curvature) {
    float hover = 0, press = 0;
    Rectangle r = {p.x, p.y, w, h};
    bool isHover = CheckCollisionPointRec(GetMousePosition(), r);
    bool isClick = isHover && IsMouseButtonDown(0);

    hover += ((isHover ? 1.0f : 0.0f) - hover) * 12 * GetFrameTime();
    press += ((isClick ? 1.0f : 0.0f) - press) * 12 * GetFrameTime();

    float s = 1 - EElasO(press) * 0.1f;
    Vector2 np = {p.x + w * (1 - s) / 2, p.y + h * (1 - s) / 2};
    
    if (isHover) {
        DrawMicaRectangle(np.x, np.y, w * s, h * s, curvature, c);
        // 覆盖一层半透明白色
        DrawMicaRectangle(np.x, np.y, w * s, h * s, curvature, ColorAlpha(WHITE, 0.15f));
    } else {
        DrawMicaRectangle(np.x, np.y, w * s, h * s, curvature, c);
    }
    DrawTextUTF(t, {np.x + w * s / 2 - MeasureText(t.c_str(), 25) / 2, np.y + h * s / 2 - 12}, 25, 0, WHITE);

    return isClick;
}

/// 绘制带图标的灯泡按钮
/// @param texture 图标纹理
/// @param position 按钮位置
/// @param width 按钮长
/// @param height 按钮宽
/// @param text 按钮颜色
/// @param color 按钮颜色
/// @param curvature 按钮圆角程度
bool DrawMicaImageButton(Texture2D texture, Vector2 position, float width, float height, string text, Color color,float curvature) {
    bool check=DrawMicaButton({position.x, position.y}, width, height, text, color, curvature);

    // 2. 计算图像缩放比例（保持宽高比，不放大图像）
    float scale = fminf(
        width / texture.width, // 水平方向最大可能缩放
        height / texture.height // 垂直方向最大可能缩放
    );
    scale = fminf(scale, 1.0f)*0.8; // 限制最大缩放比例为1（防止放大）

    // 3. 计算缩放后的图像尺寸
    float scaledWidth = texture.width * scale;
    float scaledHeight = texture.height * scale;

    // 4. 计算居中坐标（考虑缩放后的尺寸）
    Vector2 imagePosition = {
        position.x+check*10+5, // 居左
        position.y + (height - scaledHeight) / 2 // 垂直居中
    };

    // 5. 绘制缩放后的图像
    DrawTextureEx(
        texture,
        imagePosition,
        0.0f, // 旋转角度
        scale, // 缩放比例
        WHITE // 着色颜色（WHITE表示保持原色）
    );
    return check;
}

/// 绘制进度条
/// @param tg 目标进度（0 到 1）
/// @param p 进度条位置
/// @param w 进度条宽度
/// @param h 进度条高度
/// @param sp 缓动速度
/// @return 返回当前进度
float DrawMicaProgress(float &tg, Vector2 p, float w, float h, float sp) {
    static float cv = 0;
    cv += (tg - cv) * GetFrameTime() * sp;
    cv = CLAMP(cv, 0, 1);
    float pw = EElasI(cv) * w;
    DrawMicaRectangle(p.x, p.y, pw, h, h / 2, ColorAlpha(BLUE, 0.8));
    return cv;
}

#endif //NBS_H
