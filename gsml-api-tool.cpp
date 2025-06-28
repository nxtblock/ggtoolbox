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
		HWND gengenWindow = NULL;
		targetTitle = L"GenGen ToolBox";
		EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&gengenWindow));
		
		HWND gsmlWindow = NULL;
		targetTitle = L"[GSML]";
		EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&gsmlWindow));
		
		// 获取当前前台窗口
		HWND foreground = GetForegroundWindow();
		if (gengenWindow != NULL && gsmlWindow != NULL) {
			if (foreground == gengenWindow or foreground==gsmlWindow) {
				// 显示并移动 GSML 窗口
				RECT rect;
				GetWindowRect(gengenWindow, &rect);
				int right = rect.right;
				int top = rect.top;
				int bottom = rect.bottom;
				
				SetWindowPos(gsmlWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
				MoveWindow(gsmlWindow, right - 515, top + 30, 515, bottom - top - 30, TRUE);
			} else {
				// 隐藏 GSML 窗口
				ShowWindow(gsmlWindow, SW_HIDE);
				Sleep(100); 
			}
		}
		
		// 避免高CPU
	}
}

