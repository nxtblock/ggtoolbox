#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

wstring targetTitle;

// 查找单个窗口（标题匹配）
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

// 查找所有窗口（标题包含关键字）
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

// 设置窗口样式并绑定到父窗口
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

// 移动窗口到 GenGen 的右侧
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
// 获取指定窗口的进程创建时间（FILETIME 格式）
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

// 比较多个 HWND，返回运行时间最长（启动最早）的那个
HWND FindLongestRunningWindow(const std::vector<HWND>& hwndList) {
    if (hwndList.empty()) return nullptr;

    HWND longestHwnd = nullptr;
    FILETIME earliestTime = {0, 0xFFFFFFFF}; // 初始化为最大时间（避免用最小值）

    bool found = false;

    for (HWND hwnd : hwndList) {
        if (!IsWindow(hwnd)) continue; // 确保窗口仍然存在

        FILETIME creationTime;
        if (GetWindowCreationTime(hwnd, creationTime)) {
            // 第一个有效窗口直接赋值
            if (!found) {
                earliestTime = creationTime;
                longestHwnd = hwnd;
                found = true;
            } else {
                // 比较时间：如果 creationTime < earliestTime，则更早（运行更久）
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

    // 取消 GenGen 强制置顶
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
        if(gsmlWindows.size()>1){
            SendMessage(FindLongestRunningWindow(gsmlWindows), WM_SYSCOMMAND, SC_CLOSE, 0);
            // 弹出 MessageBox 前，先尝试激活桌面或前置
            FLASHWINFO fi = { sizeof(fi) };
            fi.hwnd = NULL; // 会被自动找到
            fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
            fi.uCount = 3;
            fi.dwTimeout = 0;
            FlashWindowEx(&fi); // 闪屏提醒

            // 然后弹出 MessageBox
            MessageBox(gsmlWindows[0], "GSML 窗口过多，已自动关闭之前的窗口", "错误", MB_OK | MB_TOPMOST);
        }     
        if (pclWindow != NULL) {
            // 隐藏所有 GSML
            for (HWND hwnd : gsmlWindows) {
                ShowWindow(hwnd, SW_HIDE);
            }

            // 显示并定位 PCL
            if (gengenWindow != NULL) {
                AttachAndStyle(pclWindow, gengenWindow);
                PositionWindow(pclWindow, gengenWindow);
            }
        } else {
            // 显示并定位所有 GSML
            for (HWND hwnd : gsmlWindows) {
                if (gengenWindow != NULL) {
                    AttachAndStyle(hwnd, gengenWindow);
                    PositionWindow(hwnd, gengenWindow);
                }
            }
        }

    }
}
