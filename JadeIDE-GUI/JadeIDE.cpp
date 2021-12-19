// JadeIDE-GUI.cpp : Defines the entry point for the application.
//
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "Shlwapi.lib")

#include "framework.h"
#include "JadeIDE.h"
#include <CommCtrl.h>
#include <Richedit.h>
#include <vector>
#include <strsafe.h>
#include <minwinbase.h>
#include <string>
#include <filesystem>
#include <windowsx.h>
#include "FilesFunctions.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hwndTv;
HWND hwndRedit;
HTREEITEM treeRoot = NULL;
HTREEITEM mainItem = NULL;
HTREEITEM srcItem = NULL;
LPFINFO lpCurrentFile = NULL;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_JADEIDEGUI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_JADEIDEGUI));

    MSG msg;

    // Main message loop:
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

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_JADEIDEGUI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_JADEIDEGUI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        LPHCSTRUCT hcStruct = OnCreate(hInst, hWnd);
        hwndRedit = hcStruct->hwndRedit;
        hwndTv = hcStruct->hwndTv;
    }
    break;
    case WM_CONTEXTMENU:
    {
        OnContextMenu(hInst, hWnd, hwndTv, lParam);
    }
    break;
    case WM_NOTIFY:
    {
        switch (((LPNMHDR)lParam)->code)
        {
        case NM_DBLCLK:
        {
            lpCurrentFile = OnFileOpen(hWnd, hwndRedit, hwndTv);
        }
        break;
        default:
            break;
        }
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_SAVE_FILE:
            {
                if (lpCurrentFile == NULL)
                {
                    break;
                }

                OnSaveFile(lpCurrentFile, hWnd, hwndRedit);
            }
            break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                OnExit(hWnd);
                break;
            case IDM_CREATE_PROJECT:
            {
                treeRoot = OnCreateProject(hInst, hWnd, hwndTv);
            }
            break;
            case IDM_OPEN_PROJECT:
            {
                treeRoot = OnOpenProject(hWnd, hwndTv, treeRoot);
                mainItem = LoadMainClass(hwndTv, treeRoot);
                srcItem = LoadSrcPackage(hwndTv, treeRoot);
            }
            break;
            case IDM_CLOSE_PROJECT:
            {
                treeRoot = OnCloseProject(hwndTv, treeRoot);
            }
            break;
            case IDM_CONTEXTCREATEFILE:
            {
                OnContextCreateFile(hInst, hWnd, hwndTv);
            }
            break;
            case IDM_CONTEXTCREATEPACKAGE:
            {
                OnContextCreatePackage(hInst, hWnd, hwndTv);
            }
            break;
            case IDM_CONTEXTOPEN:
            {
                lpCurrentFile = OnFileOpen(hWnd, hwndRedit, hwndTv);
            }
            break;
            case IDM_CONTEXTDELETE:
            {
                OnContextDelete(hInst, hWnd, hwndTv);
            }
            break;
            case IDM_CONTEXTRENAME:
            {
                OnContextRename(hInst, hWnd, hwndTv);
            }
            break;
            case IDM_CONTEXT_MARK_AS_MAIN:
            {
                mainItem = OnContextMarkAsMain(hInst, hWnd, hwndTv, mainItem);
            }
            break;
            case IDM_CONTEXT_MARK_AS_SRC:
            {
                srcItem = OnContextMarkAsSrc(hInst, hWnd, hwndTv, srcItem);
            }
            break;
            case IDM_BUILD_RUN:
            {
                OnBuildRun(hwndTv, treeRoot, srcItem);
            }
            break;
            case IDM_BUILD_SELECTJDK:
            {
                OnSelectJDK(hWnd, hwndTv, treeRoot);
            }
            break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            OnPaint(hWnd);
        }
        break;
    case WM_SIZE:
    {
        OnSize(hWnd, hwndTv, hwndRedit);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
