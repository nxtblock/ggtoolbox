//
// Created by lyf on 2025/4/14.
//

#ifndef NBSAPI_H
#define NBSAPI_H

#include <bits/stdc++.h>
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
    cout<<m['1'];
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
bool is_exist(const string& name) {
    FILE* file = fopen(name.c_str(), "r");
    if (file) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}
map<string,string>file_map;
void update_file(string a,string b) {
    file_map[a]=b;

    ofstream file("../data/data.txt");
    for (auto i:file_map) {
        file<<i.first<<" "<<i.second<<endl;
    }
}
void init_file() {
    ifstream file("../data/data.txt");
    string a,b;
    while (file>>a>>b) {
        file_map[a]=b;
    }
}
string query_file(string a) {
    return file_map[a];
}
#endif //NBSAPI_H
