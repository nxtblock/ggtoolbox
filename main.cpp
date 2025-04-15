#include "nbs.h"
#include "nbsapi.h"

const int screenWidth = 1280;
const int screenHeight = 720;
const int SidebarHeight = 200;
// 颜色定义
#define DARK_BACKGROUND CLITERAL(Color){40, 40, 40, 255 }
Texture2D logo,homepng;

//home  rmcl     rvcl         rgsl
//主页   MC启动器  VScode启动器  gengen脚本启动器
string now;

void mark() {
    DrawTextUTF("DEV Version", {1080, 650}, 20, 2, WHITE);
    DrawTextUTF("有些 Bug 很正常", {1080, 680}, 20, 2, WHITE);
}
string Sidebar() {
    DrawTextureEx(logo, {SidebarHeight/2-90, 20}, 0, 0.5f, WHITE);
    DrawTextUTF("GenGen", {SidebarHeight/2-30, 20}, 40, 2, WHITE);
    DrawTextUTF("ToolBox", {SidebarHeight/2-20, 50}, 25, 2, WHITE);
    // DrawMicaButton({SidebarHeight/2-100, 100}, {200, 50}, "主页", 25, WHITE, DARK_BACKGROUND);
    return now;
}
void Sign() {

    DrawMicaRectangle(SidebarHeight+25, 90, 225, 200, 0.05, DARKGRAY);
    //获取日期，年月日
    time_t now = time(0);
    tm* local_time = localtime(&now);  // 只计算一次，避免重复调用

    int month = local_time->tm_mon + 1;
    int day = local_time->tm_mday;
    // 获取星期几（0=周日，6=周六）
    int weekday = local_time->tm_wday;

    string week[]={"日","一","二","三","四","五","六"};

    if (query_file("sign")!=to_string(month)+" "+to_string(day)) {
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
            update_file("sign",to_string(month)+" "+to_string(day));
        }
    }
    else {
        DrawMicaButton({SidebarHeight+35, 220}, 205, 50, "已签到", GREEN, 0.3f);
    }


}
void Home(){
    DrawTextUTF("主页", {SidebarHeight+30, 30}, 50, 2, YELLOW);
    Sign();

    // DrawTextureEx(homepng, {SidebarHeight+270, 90}, 0, 0.5f, WHITE);
    // DrawTextUTF("欢迎使用GenGen ToolBox", {SidebarHeight+30, 250}, 30, 2, WHITE);
}
int main() {
    system("md ../data");
    init_file();
    InitWindow(screenWidth, screenHeight, "GenGen ToolBox");
    SetTargetFPS(60);

    now="home";
    logo = LoadTexture("../src/logo.png");
    homepng = LoadTexture("../src/home.png");
    SetWindowIcon(LoadImage("../src/logo.png"));
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARK_BACKGROUND);
        // 左侧主界面 (宽度300)
        DrawMicaRectangle(SidebarHeight, 0, screenWidth-SidebarHeight, screenHeight, 0.05f, Fade(BLACK, 0.5f));
        //添加 LOGO.png
        mark();
        now=Sidebar();
        if (now=="home") {
            Home();
        }

        EndDrawing();

    }

    CloseWindow();
    return 0;
}