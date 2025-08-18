#include "nbs.h"
#include "nbsapi.h"
#include <filesystem>
const int screenWidth = 1280;
const int screenHeight = 720;
const int SidebarHeight = 200;
// 颜色定义
#define DARK_BACKGROUND CLITERAL(Color){30, 30, 30, 255 }
#define GDARKGRAY CLITERAL(Color){70, 70, 70, 255 }
Texture2D logo,homepng,mcpng,loading;


string now;
map<string,Texture2D> appq;
map<string,Texture2D> apps;
map<string,Texture2D> web;
vector<string> appname={"home","gsml","running"};
map<string,string>zh_app;
void Sidebar() {
    DrawMcRectangle(0, 0, SidebarHeight-3, 720, GDARKGRAY);
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
    
    if(last_now!=now and now=="running"){
        enable_gsml_tool();
    }
    if(last_now=="running" and now!="running"){
        disable_gsml_tool();
    }
}
void Sign() {

    DrawMcRectangle(SidebarHeight+25, 90, 225, 200, GDARKGRAY);
    //获取日期，年月日
    time_t now = time(0);
    tm* local_time = localtime(&now); 

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
        if (DrawMcButton({SidebarHeight+35, 220}, 205, 50, "签到", DARKGREEN,30)==1) {
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
        DrawMcButton({SidebarHeight+35, 220}, 205, 50, "已签到", DARKGREEN,30);
    }
}
map<string,int>fileloading;
void Home(){
    DrawTextUTF("GGToolBox: HOME", {SidebarHeight+30, 30}, 30, 2, WHITE);
    Sign();
    DrawMcRectangle(SidebarHeight+25, 310, 1030, 390, GDARKGRAY);
    DrawTextureEx(homepng, {SidebarHeight+263, 87}, 0, 0.51f, WHITE);
    DrawTextUTF("快速导航", {SidebarHeight+45, 325}, 40, 2, YELLOW);
    DrawTextUTF("常用", {SidebarHeight+55, 380}, 30, 2, WHITE);
    DrawTextUTF("信息学奥赛", {SidebarHeight+305, 380}, 30, 2, WHITE);
    DrawTextUTF("Minecraft", {SidebarHeight+555, 380}, 30, 2, WHITE);
    DrawTextUTF("休闲", {SidebarHeight+805, 380}, 30, 2, WHITE);
    //在常用下的3个按钮 
    if (DrawMcImageButton(web["bi"],{SidebarHeight+55, 420}, 205, 70, "BiliBili", GRAY,25) and fileloading["BiliBili"]==0) {
        fileloading["BiliBili"]=180;
        system("start https://www.bilibili.com/");
    }
    if (DrawMcImageButton(web["cs"],{SidebarHeight+55, 500}, 205, 70, "图论画图",GRAY,25) and fileloading["图论画图"]==0) {
        fileloading["图论画图"]=180; 
        system("start https://csacademy.com/app/graph_editor/");
    }
    if (DrawMcImageButton(web["ds"],{SidebarHeight+55, 580}, 205, 70, "深度求索", GRAY,25) and fileloading["深度求索"]==0) {
        fileloading["深度求索"]=180;
        system("start https://chat.deepseek.com/");
    }
    //在信息学奥赛下的3个按钮
    if (DrawMcImageButton(web["lg"],{SidebarHeight+305, 420}, 205, 70, "洛谷", MAROON,25) and fileloading["洛谷"]==0) {
        fileloading["洛谷"]=180;
        system("start https://www.luogu.com.cn/");
    }
    if (DrawMcImageButton(web["ow"],{SidebarHeight+305, 500}, 205, 70, "OI wiki", MAROON,25) and fileloading["OI wiki"]==0) {
        fileloading["OI wiki"]=180;
        system("start https://OI.wiki/");
    }
    if (DrawMcImageButton(web["vj"],{SidebarHeight+305, 580}, 205, 70, "Vjudge", MAROON,25) and fileloading["Vjudge"]==0) {
        fileloading["Vjudge"]=180;
        system("start https://vjudge.net/"); 
    }
    //在下的3个按钮
    if (DrawMcImageButton(web["mc"],{SidebarHeight+555, 420}, 205, 70, "Mod 百科", DARKBLUE,25) and fileloading["Mod 百科"]==0) {
        fileloading["Mod 百科"]=180;
        system("start https://www.mcmod.cn/");
    }
    if (DrawMcImageButton(web["md"],{SidebarHeight+555, 500}, 205, 70, "Modrinth", DARKBLUE,25) and fileloading["Modrinth"]==0) {
        fileloading["MC 资源"]=180;
        system("start https://modrinth.com/");
    }
    if (DrawMcImageButton(web["mw"],{SidebarHeight+555, 580}, 205, 70, "MC Wiki", DARKBLUE,25) and fileloading["MC Wiki"]==0) {
        fileloading["MC Wiki"]=180;
        system("start https://zh.minecraft.wiki/");
    }
    //在 M28 下的3个按钮
    if (DrawMcImageButton(web["fl"],{SidebarHeight+805, 420}, 205, 70, "Florr.io", BROWN,25) and fileloading["Florr.io"]==0) {
        fileloading["Florr.io"]=180;
        system("start https://florr.io/");
    }
    if (DrawMcImageButton(web["ys"],{SidebarHeight+805, 500}, 205, 70, "千万别点", BROWN,25) and fileloading["Florr.io Wiki"]==0) {
        fileloading["千万别点"]=180;
        system("start https://ys.mihoyo.com/cloud/?utm_source=default#/");
    }
    if (DrawMcImageButton(web["di"],{SidebarHeight+805, 580}, 205, 70, "DigDig", BROWN,25) and fileloading["DigDig"]==0) {
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

string searchQuery = "";        // 存储用户输入的搜索词
vector<string> allFolders;      // 所有插件文件夹（初始化一次）
vector<string> filteredFolders; // 当前过滤后的插件列表
int currentPage = 0;            // 当前页码
const int itemsPerPage = 12;    // 每页最多显示 12 个插件
void initGSMLFolders() {
    if (!filesystem::exists("../gsml-main/")) return;
    allFolders.clear();
    for (const auto& entry : filesystem::directory_iterator("../gsml-main/")) {
        if (entry.is_directory()) {
            string folderName = entry.path().filename().string();
            allFolders.push_back(folderName);
        }
    }

    filteredFolders = allFolders; // 初始化为全部
}
void filterFolders(string query) {
    filteredFolders.clear();

    if (query.empty()) {
        filteredFolders = allFolders;
        return;
    }
    for(auto &i:query){
        i=tolower(i);
    }
    vector<pair<int,string>> matchIndices;
    for (auto folder : allFolders) {
        int matchCount = 0;
        size_t pos = 0;
        string y=folder;
        for(auto &i:folder){
            i=tolower(i);
        }
        for(auto i:folder){
            for (char c : query) {
                if(i==c){
                    matchCount++;
                }
            }
        }
        if (matchCount > 0) {
            matchIndices.push_back({matchCount, y});
        }
    }
    sort(matchIndices.begin(), matchIndices.end(), [](const pair<int, string>& a, const pair<int, string>& b) {
        return a.first > b.first;
    });
    filteredFolders.clear();
    for(auto i:matchIndices){
        filteredFolders.push_back(i.second);
    }
}
void gsml() {
    initGSMLFolders();
    DrawTextUTF("GGToolBox: GenGen-Script Market", {SidebarHeight + 30, 30}, 30, 2, WHITE);

    // 使用你写的输入框组件
    Vector2 inputPos = { SidebarHeight + 35, 70 };
    float inputWidth = 1000;
    int fontSize = 20;
    float spacing = 10;
    Color inputColor = DARKBLUE;
    int enableInput = 1; // 启用输入

    // 获取用户输入
    searchQuery = DrawMcInputbox(inputPos, inputWidth, fontSize, spacing, inputColor, enableInput);
    
    // 过滤插件
    filterFolders(searchQuery); // 自定义过滤函数

    int totalItems = filteredFolders.size();

    // 确保总页数至少为 1
    int totalPages = max(1, (totalItems + itemsPerPage - 1) / itemsPerPage);

    // 边界检查
    if (currentPage < 0 || currentPage >= totalPages) {
        currentPage = 0;
    }

    // 分页导航栏开始位置
    int navX = SidebarHeight + 35;
    int navY = screenHeight - 80;

    // 显示页码（包含省略号逻辑）
    vector<int> pagesToShow;

    if (totalPages <= 7) {
        for (int i = 0; i < totalPages; ++i)
            pagesToShow.push_back(i);
    } else {
        pagesToShow.push_back(0);
        if (currentPage > 3) pagesToShow.push_back(-1); // 插入省略号

        for (int i = max(1, currentPage - 2); i <= min(totalPages - 2, currentPage + 2); ++i)
            pagesToShow.push_back(i);

        if (currentPage < totalPages - 3) pagesToShow.push_back(-1);
        pagesToShow.push_back(totalPages - 1);
    }

    for (int p : pagesToShow) {
        if (p == -1) {
            DrawTextUTF("...", {(float)navX + 10, (float)navY + 5}, 20, 2, WHITE);
            navX += 40;
        } else {
            string pageNumStr = to_string(p + 1);
            Color btnColor = (p == currentPage) ? SKYBLUE : GRAY;
            if (DrawMcButton({(float)navX, (float)navY}, 40, 30, pageNumStr.c_str(), btnColor, 25)) {
                if (p >= 0 && p < totalPages) {
                    currentPage = p;
                }
            }
            navX += 50;
        }
    }

    // 绘制插件卡片区域
    int x = SidebarHeight + 35;
    int y = 120;
    int startIndex = currentPage * itemsPerPage;
    int endIndex = min(startIndex + itemsPerPage, (int)filteredFolders.size());
    
    for (int i = startIndex; i < endIndex; ++i) {
        const auto& folder = filteredFolders[i];

        if (x > screenWidth - 300) {
            x = SidebarHeight + 35;
            y += 100;
        }

        DrawMcRectangle(x, y, 250, 80, GDARKGRAY);
        DrawTextUTF(folder.c_str(), {x + 10.0f, y + 10.0f}, 20, 2, WHITE);

        string version = get_pugin("../gsml-main/" + folder + "/id.txt").second;
        string author = get_pugin("../gsml-main/" + folder + "/id.txt").first;

        DrawTextUTF(("版本: " + version).c_str(), {x + 10.0f, y + 40.0f}, 15, 2, WHITE);
        DrawTextUTF(("作者: " + author).c_str(), {x + 10.0f, y + 60.0f}, 15, 2, WHITE);

        if (DrawMcButton({x + 180.0f, y + 20.0f}, 60, 40, "运行", DARKGREEN, 25) && fileloading[folder] == 0) {
            fileloading[folder] = 180;
            run_cmd("cd \"..\\gsml-main\\" + folder + "\\\" && start \"[GSML]\" start.cmd");
            now = "running";
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
    SetTraceLogLevel(LOG_ERROR);
    SetConfigFlags(FLAG_MSAA_4X_HINT); 
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
    web["bi"]=LoadTexture("../src/web/bi.png");
    web["cs"]=LoadTexture("../src/web/cs.png");
    web["di"]=LoadTexture("../src/web/di.png");
    web["ds"]=LoadTexture("../src/web/ds.png");
    web["fl"]=LoadTexture("../src/web/fl.png");
    web["lg"]=LoadTexture("../src/web/lg.png");
    web["mc"]=LoadTexture("../src/web/mc.png");
    web["mw"]=LoadTexture("../src/web/mw.png");
    web["ow"]=LoadTexture("../src/web/ow.png");
    web["vj"]=LoadTexture("../src/web/vj.png");
    web["ys"]=LoadTexture("../src/web/ys.png");
    web["md"]=LoadTexture("../src/web/md.png");

    zh_app["home"]="主页"; 
    zh_app["gsml"]="脚本市场";
    zh_app["running"]="运行管理";
    get_gsml("https://git.ppp.ac.cn/https://github.com/nxtblock/gsml/archive/refs/heads/main.zip","../tmp.zip");
   
    for(int i=1;i<=30;i++)
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
            showmsg("错误","无法连接到脚本市场（GSML），\n你可能无法正常运行你的脚本");
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
