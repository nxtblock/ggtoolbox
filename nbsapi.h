//
// Created by lyf on 2025/4/14.
//

#ifndef NBSAPI_H
#define NBSAPI_H

#include <bits/stdc++.h>

using namespace std;

/// 将字符串中的数字转换为中文数字表示
/// @param q 输入的字符串
/// @return 转换后的字符串
string chinese_num(string q) {
    map<char,string> m;
    m['1']="一";
    m['2']="二";
    m['3']="三";
    m['4']="四";
    m['5']="五";
    m['6']="六";
    m['7']="七";
    m['8']="八";
    m['9']="九";
    m['0']="零";
    string ans="";
    for (auto i:q) {
        if ('0'<=i and i<='9') {
            ans+=m[i];

        } else {
            ans+=i;
        }
    }
    return ans;
}

/// 检查指定文件是否存在
/// @param name 文件名
/// @return 如果文件存在返回 true，否则返回 false
bool is_exist(const string& name) {
    FILE* file = fopen(name.c_str(), "r");
    if (file) {
        fclose(file);
        return true;
    } else { 
        return false;
    }
}

/// 全局文件映射，用于存储键值对
map<string,string> file_map;

/// 更新文件内容，将键值对写入文件
/// @param a 键
/// @param b 值
void update_file(string a,string b) {
    file_map[a]=b;

    ofstream file("../data/data.txt");
    for (auto i:file_map) {
        file<<i.first<<" "<<i.second<<endl;
    }
}

pair<string,string> get_pugin(string file){
    ifstream file1(file);
    string a,b;
    file1>>a>>b;
    file1.close();
    return {a,b};
}

/// 初始化文件映射，从文件中加载键值对
void init_file() {
    if (!is_exist("../data/data.txt")) {
        system("mkdir ..\\data");
        ofstream file("../data/data.txt");
        file.close();
    }
    ifstream file("../data/data.txt");
    string a,b;
    while (file>>a>>b) {
        file_map[a]=b;
    }
}


/// 查询文件映射中指定键的值
/// @param a 键
/// @return 如果键存在，返回对应的值；否则返回 "-1"
string query_file(string a) {
    if (!file_map.count(a))
        return "-1";
    return file_map[a];
}


map<string, string> getrun;         // 存子进程输出

// 启动并读取输出
void run_cmd(const string& cmdline) {
    thread([cmdline]() {
        if(getrun.count(cmdline)) {
            if (getrun[cmdline] != "") return; // 如果正在运行则不重复启动
        }
        getrun[cmdline] = ""; // 初始化输出为空
        char buffer[1024] = { '\0' };
        FILE* pf = _popen(cmdline.c_str(), "r");
        if (pf == NULL) {
            printf("open pipe failed\n");
            getrun[cmdline] = "";
            return;
        }
        string ret;
        while (fgets(buffer, sizeof(buffer), pf)) {
            ret += buffer;
            cout<<buffer;
            getrun[cmdline] = buffer;
        }
        _pclose(pf);
        getrun[cmdline] = "";
    }).detach();
}
void get_gsml(string rfile, string file) {
    // 使用 PowerShell 下载、解压，并转化 CMD/BAT 换行符为 CRLF（保留 UTF-8 编码）
    string ps_script =
        "$rfile = '" + rfile + "'; "
        "$file = '" + file + "'; "
        "Invoke-WebRequest -Uri $rfile -OutFile $file; "
        "Expand-Archive -Path $file -DestinationPath '..\\' -Force; "
        "$files = Get-ChildItem -Path '..\\' -Recurse -Include *.cmd,*.bat; "
        "foreach ($f in $files) { "
        "  $content = Get-Content -Raw -Encoding utf8 $f | Out-String; "
        "  $content = $content -replace '\\r?\\n', \"`r`n\"; "
        "  [System.IO.File]::WriteAllText($f.FullName, $content, (New-Object System.Text.UTF8Encoding($false))) "
        "}";

    string full_cmd = "powershell.exe -NoProfile -ExecutionPolicy Bypass -Command \"" + ps_script + "\"";

    run_cmd(full_cmd);
}

void enable_gsml_tool(){ 
    run_cmd("taskkill /f /im gsml-api-tool.exe & taskkill /f /im off-gsml-api-tool.exe & start /b ../gsml-api-tool.exe");
}

void disable_gsml_tool() {
    run_cmd("taskkill /f /im gsml-api-tool.exe & taskkill /f /im off-gsml-api-tool.exe & start /b ../off-gsml-api-tool.exe");
}
void exit_gsml_tool() {
    run_cmd("taskkill /f /im gsml-api-tool.exe");
    run_cmd("taskkill /f /im conhost.exe");
    run_cmd("taskkill /f /im off-gsml-api-tool.exe");
}

#endif //NBSAPI_H
