// JadeIDE.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "MainWindow.h"
#include <Windows.h>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{

    MainWindow mWnd;

    if (!mWnd.RegisterWindowClass())
    {
        exit(-1);
    }

    if (!mWnd.Create(hInstance))
    {
        exit(-1);
    }

    mWnd.Show(nCmdShow);

    MSG msg;
    HACCEL hAccelTable = mWnd.GetAccelTable();

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}
