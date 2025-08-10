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
unordered_map<HWND,int>mp;
int main() {
	HWND gengenWindow = NULL;
	targetTitle = L"GenGen ToolBox";
	EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&gengenWindow));
	
    while (1) {
        HWND gsmlWindow = NULL;
        targetTitle = L"[GSML]";
        EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&gsmlWindow));
		if(gsmlWindow and !mp.count(gsmlWindow)){
			if(mp.size()!=0){
				CloseWindow(gsmlWindow);
			}
			else{
				mp[gsmlWindow]=time(0);
			}
            for(auto i:mp){
                cout<<i.first<<" "<<i.second<<endl;
            }
		}
        bool f=0;
        if (gengenWindow != NULL && gsmlWindow != NULL) {
            // 设置 GSML 拥有者为 GenGen
            SetWindowLongPtr(gsmlWindow, GWLP_HWNDPARENT, (LONG_PTR)gengenWindow);

            // 修改扩展样式以隐藏任务栏图标
            LONG_PTR exStyle = GetWindowLongPtr(gsmlWindow, GWL_EXSTYLE);
            exStyle &= ~WS_EX_APPWINDOW;       // 移除任务栏图标
            exStyle |= WS_EX_TOOLWINDOW;       // 添加工具窗口样式
            SetWindowLongPtr(gsmlWindow, GWL_EXSTYLE, exStyle);
            SetWindowPos(gsmlWindow, NULL, 0, 0, 0, 0,
                         SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

            // 防止最小化
            if (IsIconic(gsmlWindow)) {
                ShowWindow(gsmlWindow, SW_RESTORE);
            }

            // 移动并置顶
            RECT rect;
            GetWindowRect(gengenWindow, &rect);
            int right = rect.right;
            int top = rect.top;
            int bottom = rect.bottom;

            SetWindowPos(gsmlWindow, HWND_TOP, right - 1080, top + 110, 1080, bottom - top - 110,
                         SWP_SHOWWINDOW | SWP_NOACTIVATE);
            f=1;
        } 
        //Plain Craft Launcher Community Edition
        HWND pclWindow = NULL;
        targetTitle = L"Plain Craft Launcher Community Edition";
        EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&pclWindow));

        if (gengenWindow != NULL && pclWindow != NULL) {
            // 设置 GSML 拥有者为 GenGen
            SetWindowLongPtr(pclWindow, GWLP_HWNDPARENT, (LONG_PTR)gengenWindow);

            // 修改扩展样式以隐藏任务栏图标
            LONG_PTR exStyle = GetWindowLongPtr(pclWindow, GWL_EXSTYLE);
            exStyle &= ~WS_EX_APPWINDOW;       // 移除任务栏图标
            exStyle |= WS_EX_TOOLWINDOW;       // 添加工具窗口样式
            SetWindowLongPtr(pclWindow, GWL_EXSTYLE, exStyle);
            SetWindowPos(pclWindow, NULL, 0, 0, 0, 0,
                         SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

            // 防止最小化
            if (IsIconic(pclWindow)) {
                ShowWindow(pclWindow, SW_RESTORE);
            }

            // 移动并置顶
            RECT rect;
            GetWindowRect(gengenWindow, &rect);
            int right = rect.right;
            int top = rect.top;
            int bottom = rect.bottom;

            SetWindowPos(pclWindow, HWND_TOP, right - 1080, top + 110, 1080, bottom - top - 110,
                         SWP_SHOWWINDOW | SWP_NOACTIVATE);
            f=1;
        } 
        if(f==0)
            Sleep(1000);
    }
}
