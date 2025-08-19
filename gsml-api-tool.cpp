#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

wstring targetTitle;


BOOL CALLBACK FindWindowProc(HWND hwnd, LPARAM lParam) {
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


BOOL CALLBACK FindAllWindowsProc(HWND hwnd, LPARAM lParam) {
    const int titleLength = GetWindowTextLengthW(hwnd);
    if (titleLength == 0) return TRUE;
    vector<wchar_t> titleBuffer(titleLength + 1);
    GetWindowTextW(hwnd, titleBuffer.data(), titleLength + 1);
    if (wcsstr(titleBuffer.data(), L"[GSML]") != NULL) {
        reinterpret_cast<vector<HWND>*>(lParam)->push_back(hwnd);
    }
    return TRUE;
}


void AttachAndStyle(HWND child, HWND parent) {
    SetWindowLongPtr(child, GWLP_HWNDPARENT, (LONG_PTR)parent);
    LONG_PTR exStyle = GetWindowLongPtr(child, GWL_EXSTYLE);
    exStyle &= ~WS_EX_APPWINDOW;
    exStyle |= WS_EX_TOOLWINDOW;
    SetWindowLongPtr(child, GWL_EXSTYLE, exStyle);
    SetWindowPos(child, NULL, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    if (IsIconic(child)) ShowWindow(child, SW_RESTORE);
}


void PositionWindow(HWND child, HWND gengenWindow) {
    RECT rect;
    GetWindowRect(gengenWindow, &rect);
    int right = rect.right;
    int top = rect.top;
    int bottom = rect.bottom;
    SetWindowPos(child, HWND_TOP, right - 1080, top + 110, 1080, bottom - top - 110,
                 SWP_SHOWWINDOW | SWP_NOACTIVATE);
    ShowWindow(child, SW_SHOW);
}

bool GetWindowCreationTime(HWND hwnd, FILETIME& creationTime) {
    DWORD processId;
    GetWindowThreadProcessId(hwnd, &processId);

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processId);
    if (!hProcess) return false;

    FILETIME ftCreation, ftExit, ftKernel, ftUser;
    BOOL success = GetProcessTimes(hProcess, &ftCreation, &ftExit, &ftKernel, &ftUser);
    CloseHandle(hProcess);

    if (success) {
        creationTime = ftCreation;
        return true;
    }
    return false;
}


HWND FindLongestRunningWindow(const std::vector<HWND>& hwndList) {
    if (hwndList.empty()) return nullptr;

    HWND longestHwnd = nullptr;
    FILETIME earliestTime = {0, 0xFFFFFFFF}; 

    bool found = false;

    for (HWND hwnd : hwndList) {
        if (!IsWindow(hwnd)) continue; 

        FILETIME creationTime;
        if (GetWindowCreationTime(hwnd, creationTime)) {
            
            if (!found) {
                earliestTime = creationTime;
                longestHwnd = hwnd;
                found = true;
            } else {
                
                if (CompareFileTime(&creationTime, &earliestTime) == -1) {
                    earliestTime = creationTime;
                    longestHwnd = hwnd;
                }
            }
        }
    }

    return found ? longestHwnd : nullptr;
}
int main() {
    HWND gengenWindow = NULL;
    targetTitle = L"GenGen ToolBox";
    EnumWindows(FindWindowProc, reinterpret_cast<LPARAM>(&gengenWindow));

    
    if (gengenWindow != NULL) {
        SetWindowPos(gengenWindow, HWND_NOTOPMOST, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_FRAMECHANGED);
    }

    while (true) {
        HWND pclWindow = NULL;
        targetTitle = L"Plain Craft Launcher Community Edition";
        EnumWindows(FindWindowProc, reinterpret_cast<LPARAM>(&pclWindow));

        vector<HWND> gsmlWindows;
        EnumWindows(FindAllWindowsProc, reinterpret_cast<LPARAM>(&gsmlWindows));
        if (pclWindow != NULL) {
            
            for (HWND hwnd : gsmlWindows) {
                ShowWindow(hwnd, SW_HIDE);
            }

            
            if (gengenWindow != NULL) {
                AttachAndStyle(pclWindow, gengenWindow);
                PositionWindow(pclWindow, gengenWindow);
            }
        } else {
            
            for (HWND hwnd : gsmlWindows) {
                if (gengenWindow != NULL) {
                    AttachAndStyle(hwnd, gengenWindow);
                    PositionWindow(hwnd, gengenWindow);
                }
            }
        }
        if(gsmlWindows.size()>1){
            SendMessage(FindLongestRunningWindow(gsmlWindows), WM_SYSCOMMAND, SC_CLOSE, 0);
            
            FLASHWINFO fi = { sizeof(fi) };
            fi.hwnd = NULL; 
            fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
            fi.uCount = 3;
            fi.dwTimeout = 0;
            FlashWindowEx(&fi); 

            MessageBox(gsmlWindows[0], "GSML 窗口过多，已自动关闭之前的窗口", "错误", MB_OK | MB_TOPMOST);
        }     
    }
}
