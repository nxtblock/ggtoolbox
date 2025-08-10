#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

wstring targetTitle;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    HWND* pFoundWindow = reinterpret_cast<HWND*>(lParam);
    
    const int titleLength = GetWindowTextLengthW(hwnd);
    if (titleLength == 0) return TRUE;
    
    vector<wchar_t> titleBuffer(titleLength + 1);
    GetWindowTextW(hwnd, titleBuffer.data(), titleLength + 1);
    
    if (wcsstr(titleBuffer.data(), targetTitle.c_str()) != NULL) {
        *pFoundWindow = hwnd;
        return FALSE;
    }
    
    return TRUE;
}

int main() {
    while (1) {
        HWND gsmlWindow = NULL;
        targetTitle = L"[GSML]";
        EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&gsmlWindow));
        HWND pclWindow = NULL;
        targetTitle = L"Plain Craft Launcher Community Edition";
        EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&pclWindow));
        if (gsmlWindow != NULL) {
            ShowWindow(gsmlWindow, SW_HIDE);
        }
        if (pclWindow != NULL) {
            ShowWindow(pclWindow, SW_HIDE);
        }
        
        Sleep(5000);
    }
}
