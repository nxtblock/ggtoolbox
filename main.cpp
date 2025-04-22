#include "nbs.h"
#include "nbsapi.h"

const int screenWidth = 1280;
const int screenHeight = 720;
const int SidebarHeight = 200;
// 颜色定义
#define DARK_BACKGROUND CLITERAL(Color){40, 40, 40, 255 }
Texture2D logo,homepng,mcpng,loading;



void mark() {
    DrawTextUTF("DEV Version", {1080, 650}, 20, 2, WHITE);
    DrawTextUTF("有些 Bug 很正常", {1080, 680}, 20, 2, WHITE);
}
//home  rmcl      jsml
//主页   MC启动器   gengen脚本启动器
string now;
map<string,Texture2D> appq;
map<string,Texture2D> apps;
vector<string> appname={"home","rmcl","jsml"};
map<string,string>zh_app;
string Sidebar() {
    DrawTextureEx(logo, {SidebarHeight/2-90, 20}, 0, 0.5f, WHITE);
    DrawTextUTF("GenGen", {SidebarHeight/2-30, 20}, 40, 2, WHITE);
    DrawTextUTF("ToolBox", {SidebarHeight/2-20, 50}, 25, 2, WHITE);

    float y=100;
    for (auto i:appname) {
        if (i==now) {
            DrawMicaImageButton( appq[i],{0, y},SidebarHeight,50, zh_app[i],DARKBLUE, 0.3f);
        }
        else {
            if (DrawMicaImageButton( apps[i],{0, y},SidebarHeight,50, zh_app[i],DARK_BACKGROUND, 0.3f)) {
                now=i;
            }
        }
        y+=55;
    }

    return now;
}
void Sign() {

    DrawMicaRectangle(SidebarHeight+25, 90, 225, 200, 0.1, DARKGRAY);
    //获取日期，年月日
    time_t now = time(0);
    tm* local_time = localtime(&now);  // 只计算一次，避免重复调用

    int month = local_time->tm_mon + 1;
    int day = local_time->tm_mday;
    // 获取星期几（0=周日，6=周六）
    int weekday = local_time->tm_wday;

    string week[]={"日","一","二","三","四","五","六"};
    if (query_file("sign").find(to_string(month)+"-"+to_string(day))==-1) {
        //签到 类似于洛谷主页
        if (month>=10) {
            DrawTextUTF(to_string(month), {SidebarHeight+50, 110}, 30, 2, GREEN);
        }
        else {
            DrawTextUTF(chinese_num(to_string(month)), {SidebarHeight+50, 110}, 30, 2, GREEN);
        }
        DrawTextUTF("月", {SidebarHeight+50, 140}, 30, 2, GREEN);
        if (month==2 or month==4 or month==6 or month==9 or month==11)
            DrawTextUTF("小", {SidebarHeight+50, 170}, 30, 2, GREEN);
        else
            DrawTextUTF("大", {SidebarHeight+50, 170}, 30, 2, GREEN);
        DrawTextUTF("星", {SidebarHeight+200, 110}, 30, 2, GREEN);
        DrawTextUTF("期", {SidebarHeight+200, 140}, 30, 2, GREEN);
        DrawTextUTF(week[weekday], {SidebarHeight+200, 170}, 30, 2, GREEN);

        string sday=to_string(day);
        if (sday.size()==1)
            sday="0"+sday;
        DrawTextUTF(sday, {SidebarHeight+95, 100}, 100, 2, WHITE);
        if (DrawMicaButton({SidebarHeight+35, 220}, 205, 50, "签到", GREEN, 0.3f)==1) {
            random_device fate;
            update_file("sign",to_string(month)+"-"+to_string(day)+"-"+to_string(fate()%3));
        }
    }
    else {
        string file=query_file("sign");
        int last_fate=stoi(file.substr(file.find("-", file.find("-")+1)+1));
        if (last_fate == 0) {
            DrawTextUTF("$大吉$", {SidebarHeight+50, 110}, 80, 2, RED);
        } else if (last_fate == 1) {
            DrawTextUTF("$中平$", {SidebarHeight+50, 110}, 80, 2, GREEN);
        } else if (last_fate == 2) {
            DrawTextUTF("$大凶$", {SidebarHeight+50, 110}, 80, 2, WHITE);
        }
        DrawMicaButton({SidebarHeight+35, 220}, 205, 50, "已签到", GREEN, 0.3f);
    }


}
void Home(){
    DrawTextUTF("主页", {SidebarHeight+30, 30}, 50, 2, YELLOW);
    Sign();

    DrawTextureEx(homepng, {SidebarHeight+275, 90}, 0, 0.5f, WHITE);

    DrawMicaRectangle(SidebarHeight+25, 310, 1030, 390, 0.05, DARKGRAY);
    DrawTextUTF("快速导航", {SidebarHeight+35, 310}, 40, 2, YELLOW);
    DrawTextUTF("常用", {SidebarHeight+35, 380}, 30, 2, WHITE);
    DrawTextUTF("信息学奥赛", {SidebarHeight+285, 380}, 30, 2, WHITE);
    DrawTextUTF("Minecraft", {SidebarHeight+535, 380}, 30, 2, WHITE);
    DrawTextUTF("Florr.io", {SidebarHeight+785, 380}, 30, 2, WHITE);
    //在常用下的3个按钮
    if (DrawMicaButton({SidebarHeight+35, 420}, 205, 70, "Notion 笔记", MAROON, 0.3f)) {
        system("start https://www.notion.so/");
    }

    if (DrawMicaButton({SidebarHeight+35, 500}, 205, 70, "博客园",MAROON, 0.3f)) {
        system("start https://www.cnblogs.com/");
    }

    if (DrawMicaButton({SidebarHeight+35, 580}, 205, 70, "深度求知", MAROON, 0.3f)) {
        system("start https://chat.deepseek.com/");
    }
    //在信息学奥赛下的3个按钮
    if (DrawMicaButton({SidebarHeight+285, 420}, 205, 70, "洛谷", DARKBLUE, 0.3f)) {
        system("start https://www.luogu.com.cn/");
    }
    if (DrawMicaButton({SidebarHeight+285, 500}, 205, 70, "OI wiki", DARKBLUE, 0.3f)) {
        system("start https://OI.wiki/");
    }
    if (DrawMicaButton({SidebarHeight+285, 580}, 205, 70, "Vjudge", DARKBLUE, 0.3f)) {
        system("start https://vjudge.net/");
    }
    //在Minecraft下的3个按钮
    if (DrawMicaButton({SidebarHeight+535, 420}, 205, 70, "Mod 百科", DARKGREEN, 0.3f)) {
        system("start https://www.mcmod.cn/");
    }
    if (DrawMicaButton({SidebarHeight+535, 500}, 205, 70, "MC 资源", DARKGREEN, 0.3f)) {
        system("start https://modrinth.com/");
    }
    if (DrawMicaButton({SidebarHeight+535, 580}, 205, 70, "MC Wiki", DARKGREEN, 0.3f)) {
        system("start https://zh.minecraft.wiki/");
    }

    //在Florr.io下的3个按钮
    if (DrawMicaButton({SidebarHeight+785, 420}, 205, 70, "Florr.io", DARKPURPLE, 0.3f)) {
        system("start https://florr.io/");
    }
    if (DrawMicaButton({SidebarHeight+785, 500}, 205, 70, "Florr.io Wiki", DARKPURPLE, 0.3f)) {
        system("start https://florrio.fandom.com/zh/wiki/%E9%A6%96%E9%A1%B5");
    }
    if (DrawMicaButton({SidebarHeight+785, 580}, 205, 70, "DigDig", DARKPURPLE, 0.3f)) {
        system("start https://digdig.io/");
    }
}
void RMCL() {
    DrawTextUTF("RMCL", {SidebarHeight+30, 30}, 50, 2, YELLOW);
    DrawMicaButton({SidebarHeight+35, 60}, 100, 30, "启动", BLACK, 0.1f);
    DrawMicaButton({SidebarHeight+135, 60}, 100, 30, "设置", BLACK, 0.1f);

    // DrawTextureEx(mcpng, {SidebarHeight+30, 100}, 0, 0.5f, WHITE);
}
int main() {
    InitWindow(screenWidth, screenHeight, "GenGen ToolBox");
    SetTargetFPS(60);
    loading=LoadTexture("../src/loading.png");
    BeginDrawing();
    ClearBackground(DARK_BACKGROUND);
    DrawTextureEx(loading, {0,0}, 0, 1.0f, WHITE);
    EndDrawing();

    fntFileData = LoadFileData("../src/NotoSansSC.ttf", &fntFileSize);
    init_file();
    now="home";
    logo = LoadTexture("../src/logo.png");
    homepng = LoadTexture("../src/home.png");
    mcpng= LoadTexture("../src/mcv.png");
    appq["home"] = LoadTexture("../src/app/qh.png");
    appq["jsml"] = LoadTexture("../src/app/qj.png");
    appq["rmcl"] = LoadTexture("../src/app/qm.png");
    apps["home"] = LoadTexture("../src/app/sh.png");
    apps["jsml"] = LoadTexture("../src/app/sj.png");
    apps["rmcl"] = LoadTexture("../src/app/sm.png");
    SetWindowIcon(LoadImage("../src/logo.png"));
    zh_app["home"]="主页";
    zh_app["rmcl"]="RMCL";
    zh_app["jsml"]="GG脚本市场";
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARK_BACKGROUND);
        // 左侧主界面 (宽度300)
        DrawMicaRectangle(SidebarHeight, 0, screenWidth-SidebarHeight, screenHeight, 0.03f, Fade(BLACK, 0.5f));
        //添加 LOGO.png
        mark();
        now=Sidebar();
        if (now=="home") {
            Home();
        }
        if (now=="rmcl") {
            RMCL();
        }
        EndDrawing();

    }

    CloseWindow();
    return 0;
}