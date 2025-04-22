//
// Created by lyf on 2025/4/14.
//

#ifndef NBSAPI_H
#define NBSAPI_H

#include <bits/stdc++.h>

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

/// 初始化文件映射，从文件中加载键值对
void init_file() {
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

#endif //NBSAPI_H

