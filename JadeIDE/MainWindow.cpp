#include "framework.h"
#include "MainWindow.h"
#include <Windows.h>

LPCWSTR MainWindow::className = L"MainWindow";
LPCWSTR MainWindow::title = L"Jade IDE";

MainWindow::MainWindow()
{

}

MainWindow::~MainWindow()
{

}

ATOM MainWindow::RegisterWindowClass()
{
    HINSTANCE hInstance = GetModuleHandle(0);
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = MainWindow::MainWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_JADEIDE));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_JADEIDE);
    wcex.lpszClassName = MainWindow::className;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL MainWindow::Create(HINSTANCE hInst)
{
    HWND hWnd = CreateWindowW(MainWindow::className, MainWindow::title, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInst, nullptr);

    MainWindow::handle = hWnd;

    MainWindow::hAccelTable = LoadAccelerators(hInst, MAKEINTRESOURCE(IDC_JADEIDE));

    return hWnd != 0;
}

void MainWindow::Show(int cmdShow)
{
    ShowWindow(MainWindow::handle, cmdShow);
    UpdateWindow(MainWindow::handle);
}

HWND MainWindow::GetHandle()
{
    return handle;
}

HACCEL MainWindow::GetAccelTable()
{
    return hAccelTable;
}

LRESULT __stdcall MainWindow::MainWndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_NCCREATE) {
        MainWindow* mWnd = new MainWindow();
        ::SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG>(mWnd));
        mWnd->handle = handle;
        return DefWindowProc(handle, message, wParam, lParam);
    }

    HINSTANCE hInst = (HINSTANCE)GetWindowLongW(handle, GWL_HINSTANCE);

    MainWindow* mWnd = reinterpret_cast<MainWindow*>(::GetWindowLongPtr(handle, GWLP_USERDATA));

    switch (message)
    {
    case WM_CREATE:
    {
        mWnd->OnCreate();
        return DefWindowProc(handle, message, wParam, lParam);
    }
    break;
    case WM_NOTIFY:
    {

    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), handle, MainWindow::About);
            break;
        case IDM_EXIT:
            DestroyWindow(handle);
            break;
        default:
            return DefWindowProc(handle, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(handle, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(handle, &ps);
    }
    break;
    case WM_SIZE:
    {
        mWnd->OnSize();
    }
    break;
    case WM_DESTROY:
    {
        MainWindow* mWnd = reinterpret_cast<MainWindow*>(::GetWindowLongPtr(handle, GWLP_USERDATA));
        delete mWnd;
        PostQuitMessage(0);
    }
    break;
    default:
        return DefWindowProc(handle, message, wParam, lParam);
    }
    return 0;
}

INT_PTR __stdcall MainWindow::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void MainWindow::OnCreate()
{

}

void MainWindow::OnSize()
{

}