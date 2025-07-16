#include "nbs.h"
#include "nbsapi.h"
#include <filesystem>

const int screenWidth = 1280;
const int screenHeight = 720;
const int SidebarHeight = 200;
// 颜色定义
#define DARK_BACKGROUND CLITERAL(Color){40, 40, 40, 255 }
Texture2D logo,homepng,mcpng,loading;


string now;
map<string,Texture2D> appq;
map<string,Texture2D> apps;
vector<string> appname={"home","gsml","running"};
map<string,string>zh_app;
void Sidebar() {
    DrawMcRectangle(0, 0, SidebarHeight-3, 720, DARKGRAY);
    DrawTextureEx(logo, {SidebarHeight/2-90, 20}, 0, 0.5f, WHITE);
    DrawTextUTF("GenGen", {SidebarHeight/2-30, 20}, 35, 2, WHITE);
    DrawTextUTF("ToolBox", {SidebarHeight/2-20, 50}, 25, 2, WHITE);
    string last_now=now;
    float y=100;
    for (auto i:appname) {
        if (i==now) {
            DrawMcImageButton( appq[i],{3, y},SidebarHeight-6,50, zh_app[i],SKYBLUE);
        }
        else {
            if (DrawMicaImageButton( apps[i],{0, y},SidebarHeight,50, zh_app[i],BLANK,0.1f)) {
                now=i;
            }
        } 
        y+=55;
    }
    
    if (IsKeyPressed(KEY_HOME)) {
        now="home";
    }   
    
    if (IsKeyPressed(KEY_ONE)) {
        now="home";
    }   
    if (IsKeyPressed(KEY_TWO)) {
        now="gsml";
    }
    if(IsKeyPressed(KEY_THREE)) {
        now="running";
    }
    if(last_now!=now and now=="running"){
        enable_gsml_tool();
    }
    if(last_now=="running" and now!="running"){
        disable_gsml_tool();
    }
}
void Sign() {

    DrawMcRectangle(SidebarHeight+25, 90, 225, 200, DARKGRAY);
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
        if (DrawMcButton({SidebarHeight+35, 220}, 205, 50, "签到", Fade(GREEN,0.7f),30)==1) {
            random_device fate;
            update_file("sign",to_string(month)+"-"+to_string(day)+"-"+to_string(fate()%3));
        }
    }
    else {
        string file=query_file("sign");
        int last_fate=stoi(file.substr(file.find("-", file.find("-")+1)+1));
        if (last_fate == 0) {
            DrawTextUTF("$大吉$", {SidebarHeight+32, 110}, 80, 2, RED);
        } else if (last_fate == 1) {
            DrawTextUTF("$中平$", {SidebarHeight+32, 110}, 80, 2, GREEN);
        } else if (last_fate == 2) {
            DrawTextUTF("$大凶$", {SidebarHeight+32, 110}, 80, 2, WHITE);
        }
        DrawMcButton({SidebarHeight+35, 220}, 205, 50, "已签到", GREEN,30);
    }
}
map<string,int>fileloading;
void Home(){
    DrawTextUTF("GGToolBox: HOME", {SidebarHeight+30, 30}, 30, 2, WHITE);
    Sign();
    DrawMcRectangle(SidebarHeight+25, 310, 1030, 390, DARKGRAY);
    DrawTextureEx(homepng, {SidebarHeight+263, 87}, 0, 0.51f, WHITE);
    DrawTextUTF("快速导航", {SidebarHeight+35, 315}, 40, 2, YELLOW);
    DrawTextUTF("常用", {SidebarHeight+35, 380}, 30, 2, WHITE);
    DrawTextUTF("信息学奥赛", {SidebarHeight+285, 380}, 30, 2, WHITE);
    DrawTextUTF("Minecraft", {SidebarHeight+535, 380}, 30, 2, WHITE);
    DrawTextUTF("Florr.io", {SidebarHeight+785, 380}, 30, 2, WHITE);
    //在常用下的3个按钮 
    if (DrawMcButton({SidebarHeight+35, 420}, 205, 70, "BiliBili", RED,25) and fileloading["BiliBili"]==0) {
        fileloading["BiliBili"]=180;
        system("start https://www.bilibili.com/");
    }
    if (DrawMcButton({SidebarHeight+35, 500}, 205, 70, "博客园",RED,25) and fileloading["博客园"]==0) {
        fileloading["博客园"]=180; 
        system("start https://www.cnblogs.com/");
    }
    if (DrawMcButton({SidebarHeight+35, 580}, 205, 70, "深度求知", RED,25) and fileloading["深度求知"]==0) {
        fileloading["深度求知"]=180;
        system("start https://chat.deepseek.com/");
    }
    //在信息学奥赛下的3个按钮
    if (DrawMcButton({SidebarHeight+285, 420}, 205, 70, "洛谷", GOLD,25) and fileloading["洛谷"]==0) {
        fileloading["洛谷"]=180;
        system("start https://www.luogu.com.cn/");
    }
    if (DrawMcButton({SidebarHeight+285, 500}, 205, 70, "OI wiki", GOLD,25) and fileloading["OI wiki"]==0) {
        fileloading["OI wiki"]=180;
        system("start https://OI.wiki/");
    }
    if (DrawMcButton({SidebarHeight+285, 580}, 205, 70, "Vjudge", GOLD,25) and fileloading["Vjudge"]==0) {
        fileloading["Vjudge"]=180;
        system("start https://vjudge.net/"); 
    }
    //在Minecraft下的3个按钮
    if (DrawMcButton({SidebarHeight+535, 420}, 205, 70, "Mod 百科", BLUE,25) and fileloading["Mod 百科"]==0) {
        fileloading["Mod 百科"]=180;
        system("start https://www.mcmod.cn/");
    }
    if (DrawMcButton({SidebarHeight+535, 500}, 205, 70, "Modrinth", BLUE,25) and fileloading["Modrinth"]==0) {
        fileloading["MC 资源"]=180;
        system("start https://modrinth.com/");
    }
    if (DrawMcButton({SidebarHeight+535, 580}, 205, 70, "MC Wiki", BLUE,25) and fileloading["MC Wiki"]==0) {
        fileloading["MC Wiki"]=180;
        system("start https://zh.minecraft.wiki/");
    }
    //在Florr.io下的3个按钮
    if (DrawMcButton({SidebarHeight+785, 420}, 205, 70, "Florr.io", PURPLE,25) and fileloading["Florr.io"]==0) {
        fileloading["Florr.io"]=180;
        system("start https://florr.io/");
    }
    if (DrawMcButton({SidebarHeight+785, 500}, 205, 70, "Florr.io Wiki", PURPLE,25) and fileloading["Florr.io Wiki"]==0) {
        fileloading["Florr.io Wiki"]=180;
        system("start https://florrio.fandom.com/zh/wiki/");
    }
    if (DrawMcButton({SidebarHeight+785, 580}, 205, 70, "DigDig", PURPLE,25) and fileloading["DigDig"]==0) {
        fileloading["DigDig"]=180;
        system("start https://digdig.io/");
    }
}
void showmsg(string title,string msg){
    DrawMcRectangle(screenWidth-320, screenHeight-130, 300, 100, DARKBLUE);
    DrawTextUTF(title, {screenWidth-310, screenHeight-120}, 30, 2, YELLOW);
    int maxWidth = 280;
    int fontSize = 20;
    int lineHeight = 28;
    vector<string> lines;
    string curLine, curWord;
    int curWidth = 0;
    for (int i = 0; i <= msg.size(); ++i) {
        char c = (i < msg.size() ? msg[i] : ' ');
        if (c == '\n' or c == ' ' or i == msg.size()) {
            int wordWidth = MeasureTextEx(GetFontDefault(), curWord.c_str(), fontSize, 2).x;
            if (curWidth + wordWidth > maxWidth) {
                if (!curLine.empty()) lines.push_back(curLine);
                curLine = curWord;
                curWidth = wordWidth;
            } else {
                curLine += curWord;
                curWidth += wordWidth;
            }
            if (c == ' ' or c == '\n') { 
                int spaceWidth = MeasureTextEx(GetFontDefault(), " ", fontSize, 2).x;
                if (curWidth + spaceWidth > maxWidth) {
                    lines.push_back(curLine);
                    curLine = "";
                    curWidth = 0;
                } else {
                    curLine += " ";
                    curWidth += spaceWidth;
                }
            }
            if (c == '\n') {
                lines.push_back(curLine);
                curLine = "";
                curWidth = 0;
            }
            curWord = "";
        } else {
            curWord += c;
        }
    }
    if (!curLine.empty()) lines.push_back(curLine);

    // 最多显示 2 行，超出加...
    int maxLines = 2;
    if ((int)lines.size() > maxLines) {
        lines.resize(maxLines);
        string &last = lines.back();
        // 截断最后一行加...
        while (MeasureTextEx(GetFontDefault(), (last + "...").c_str(), fontSize, 2).x > maxWidth && !last.empty()) {
            last.pop_back();
        }
        last += "...";
    }
    for (int i = 0; i < lines.size(); ++i) {
        DrawTextUTF(lines[i], {screenWidth-310, (float)(screenHeight-80 + (int)i*lineHeight)}, fontSize, 2, WHITE);
    }
}
int stoptps=0,errortps=0,wait=0;
int flag=0;
void gsml() {
    DrawTextUTF("GGToolBox: GenGen-Script Market", {SidebarHeight+30, 30}, 30, 2, WHITE);
    //查找..gsml-main/的子文件夹
    vector<string> folders;
    if (!std::filesystem::exists("../gsml-main/")) {
        DrawTextUTF("正在加载插件库...", {SidebarHeight+400, 300}, 40, 2, YELLOW);
        return;
    }
    for (const auto& entry : std::filesystem::directory_iterator("../gsml-main/")) {
        if (entry.is_directory()) {
            folders.push_back(entry.path().filename().string());
        }
    }
    //每一个子文件夹都是一个插件，绘制插件，每行2个，左侧是名字，右侧是运行按钮和版本号，作者
    int x=SidebarHeight+35,y=100;
    for (const auto& folder : folders) {
        if (x > screenWidth - 300) {
            x = SidebarHeight + 35;
            y += 100;
        }
        DrawMcRectangle(x, y, 250, 80, DARKGRAY);
        DrawTextUTF(folder, {x+10.0f, y+10.0f}, 20, 2, WHITE);
        string version = get_pugin("../gsml-main/"+folder+"/id.txt").second;
        string author = get_pugin("../gsml-main/"+folder+"/id.txt").first;
        DrawTextUTF("版本: " + version, {x+10.0f, y+40.0f}, 15, 2, WHITE);
        DrawTextUTF("作者: " + author, {x+10.0f, y+60.0f}, 15, 2, WHITE);
        if (DrawMcButton({x+180.0f, y+20.0f}, 60, 40, "运行", GREEN, 25) and fileloading[folder]==0) {
            fileloading[folder] = 180;
            run_cmd("cd \"..\\gsml-main\\"+folder+"\\\" && start \"[GSML]\" start.cmd");
            now="running";
            enable_gsml_tool();
        } 
        x += 300;
    }
    
}
void running(){
    
    DrawTextUTF("GGToolBox: Script run management", {SidebarHeight+30, 30}, 30, 2, WHITE);
    
    DrawTextUTF("你还没有运行插件", {SidebarHeight+400, 300}, 40, 2, YELLOW);
}
int main() {
    int iserror = 0;
    InitWindow(screenWidth, screenHeight, "GenGen ToolBox");
    SetTargetFPS(60);
    loading=LoadTexture("../src/loading.png");
    SetWindowIcon(LoadImage("../src/logo.png"));
    BeginDrawing();
    ClearBackground(DARK_BACKGROUND);
    DrawTextureEx(loading, {0,0}, 0, 1.0f, WHITE);
    EndDrawing();

    init_file();
    now="home";
    logo = LoadTexture("../src/logo.png");
    homepng = LoadTexture("../src/home.png");
    mcpng= LoadTexture("../src/mcv.png");
    appq["home"] = LoadTexture("../src/app/sprite0.png");
    appq["gsml"] = LoadTexture("../src/app/sprite1.png");
    appq["running"] = LoadTexture("../src/app/sprite2.png");
    apps["home"] = LoadTexture("../src/app/sprite3.png");
    apps["gsml"] = LoadTexture("../src/app/sprite4.png");
    apps["running"] = LoadTexture("../src/app/sprite5.png");
    zh_app["home"]="主页"; 
    zh_app["gsml"]="脚本市场";
    zh_app["running"]="运行管理";
    if(!system("ping www.baidu.com -n 1")){
        get_gsml("https://git.ppp.ac.cn/https://github.com/nxtblock/gsml/archive/refs/heads/main.zip","../tmp.zip");
    }
    else{
        iserror=210; 
    } 
    
    for(int i=1;i<=3;i++)
        while(!InitFontSystem("../src/DouyinSansBold.otf"));
    for (float fade=1.0f;fade>0.0f;fade-=0.02f) {
        BeginDrawing();
        ClearBackground(DARK_BACKGROUND);
        Sidebar();
        Home();
        DrawTextureEx(loading, {0, 0}, 0, 1.0f, ColorAlpha(WHITE, fade));
        EndDrawing();
    }
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARK_BACKGROUND);
        Sidebar();
        if (now=="home") {
            Home();
        }
        if (now=="gsml") {
            gsml();
        }
        if(now=="running"){
            running();
        }
        if(iserror){
            showmsg("错误","无法连接到脚本市场（GSML），你可能无法正常运行你的脚本");
            iserror--;
        }
        
        EndDrawing();
        for(auto &i:fileloading){
            if(i.second>0)
                i.second--;
        }
    }
    exit_gsml_tool();
    UnloadFontSystem();
    CloseWindow();
    return 0;
}
