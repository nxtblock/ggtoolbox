#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

wstring targetTitle;

// ���ҵ������ڣ�����ƥ�䣩
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

// �������д��ڣ���������ؼ��֣�
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

// ���ô�����ʽ���󶨵�������
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

// �ƶ����ڵ� GenGen ���Ҳ�
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
// ��ȡָ�����ڵĽ��̴���ʱ�䣨FILETIME ��ʽ��
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

// �Ƚ϶�� HWND����������ʱ������������磩���Ǹ�
HWND FindLongestRunningWindow(const std::vector<HWND>& hwndList) {
    if (hwndList.empty()) return nullptr;

    HWND longestHwnd = nullptr;
    FILETIME earliestTime = {0, 0xFFFFFFFF}; // ��ʼ��Ϊ���ʱ�䣨��������Сֵ��

    bool found = false;

    for (HWND hwnd : hwndList) {
        if (!IsWindow(hwnd)) continue; // ȷ��������Ȼ����

        FILETIME creationTime;
        if (GetWindowCreationTime(hwnd, creationTime)) {
            // ��һ����Ч����ֱ�Ӹ�ֵ
            if (!found) {
                earliestTime = creationTime;
                longestHwnd = hwnd;
                found = true;
            } else {
                // �Ƚ�ʱ�䣺��� creationTime < earliestTime������磨���и��ã�
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

    // ȡ�� GenGen ǿ���ö�
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
            // ���� MessageBox ǰ���ȳ��Լ��������ǰ��
            FLASHWINFO fi = { sizeof(fi) };
            fi.hwnd = NULL; // �ᱻ�Զ��ҵ�
            fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
            fi.uCount = 3;
            fi.dwTimeout = 0;
            FlashWindowEx(&fi); // ��������

            // Ȼ�󵯳� MessageBox
            MessageBox(gsmlWindows[0], "GSML ���ڹ��࣬���Զ��ر�֮ǰ�Ĵ���", "����", MB_OK | MB_TOPMOST);
        }     
        if (pclWindow != NULL) {
            // �������� GSML
            for (HWND hwnd : gsmlWindows) {
                ShowWindow(hwnd, SW_HIDE);
            }

            // ��ʾ����λ PCL
            if (gengenWindow != NULL) {
                AttachAndStyle(pclWindow, gengenWindow);
                PositionWindow(pclWindow, gengenWindow);
            }
        } else {
            // ��ʾ����λ���� GSML
            for (HWND hwnd : gsmlWindows) {
                if (gengenWindow != NULL) {
                    AttachAndStyle(hwnd, gengenWindow);
                    PositionWindow(hwnd, gengenWindow);
                }
            }
        }

    }
}
