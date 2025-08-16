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
