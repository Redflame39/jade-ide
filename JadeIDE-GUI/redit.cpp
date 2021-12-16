#include "redit.h"

HWND CreateRichEdit(HWND hwndOwner,        // Dialog box handle.
    HINSTANCE hinst)       // Application or DLL instance.
{
    LoadLibrary(TEXT("Msftedit.dll"));

    RECT cRect;
    GetClientRect(hwndOwner, &cRect);
    HWND hwndEdit = CreateWindowEx(0, MSFTEDIT_CLASS, TEXT("Type here"),
        ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP | WS_SIZEBOX,
        cRect.right * 0.25, cRect.top, cRect.right, cRect.bottom,
        hwndOwner, NULL, hinst, NULL);

    return hwndEdit;
}

BOOL SetTextToREdit(HWND hWnd, HWND hwndRedit, TCHAR* text)
{
    return SetWindowText(hwndRedit, (LPCWSTR)text);
}

TCHAR* GetREditText(HWND hWnd, HWND hwndRedit)
{
    int length = GetWindowTextLength(hwndRedit) + 1;
    TCHAR* text = (TCHAR*)GlobalAlloc(GPTR, length);
    GetWindowText(hwndRedit, text, length);
    return text;
}