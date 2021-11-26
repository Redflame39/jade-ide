// JadeIDE-GUI.cpp : Defines the entry point for the application.
//
#pragma comment(lib, "comctl32.lib")

#include "framework.h"
#include "JadeIDE.h"
#include <CommCtrl.h>
#include <Richedit.h>
#include <shlobj_core.h>
#include <vector>
#include <strsafe.h>
#include <minwinbase.h>
#include <string>
#include <filesystem>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hwndTv;
HTREEITEM treeRoot = NULL;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

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

LPWSTR GetBaseName(LPTSTR fullpath)
{
    std::wstring w_fp(fullpath);
    const int copyFrom = w_fp.find_last_of(L"/\\") + 1;
    const int copyCount = w_fp.length() - copyFrom;
    wchar_t* buffer = new wchar_t(copyCount);
    w_fp.copy(buffer, copyCount, copyFrom);
    buffer[copyCount] = '\0';
    return buffer;
    //std::wstring w_fp_substr = w_fp.substr(w_fp.find_last_of(L"/\\") + 1);
}

HWND CreateRichEdit(HWND hwndOwner,        // Dialog box handle.
    int x, int y,          // Location.
    int width, int height, // Dimensions.
    HINSTANCE hinst)       // Application or DLL instance.
{
    LoadLibrary(TEXT("Msftedit.dll"));

    HWND hwndEdit = CreateWindowEx(0, MSFTEDIT_CLASS, TEXT("Type here"),
        ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP,
        x, y, width, height,
        hwndOwner, NULL, hinst, NULL);

    return hwndEdit;
}

// Create a tree-view control. 
// Returns the handle to the new control if successful,
// or NULL otherwise. 
// hwndParent - handle to the control's parent window. 
// lpszFileName - name of the file to parse for tree-view items.
// g_hInst - the global instance handle.
// ID_TREEVIEW - the resource ID of the control.

HWND CreateATreeView(HWND hwndParent)
{
    RECT rcClient;  // dimensions of client area 
    HWND hwndTV;    // handle to tree-view control 

    // Ensure that the common control DLL is loaded. 
    INITCOMMONCONTROLSEX config;
    config.dwSize = sizeof(INITCOMMONCONTROLSEX);
    config.dwICC = ICC_TREEVIEW_CLASSES;
    InitCommonControlsEx(&config);

    // Get the dimensions of the parent window's client area, and create 
    // the tree-view control. 
    GetClientRect(hwndParent, &rcClient);
    hwndTV = CreateWindowEx(0,
        WC_TREEVIEW,
        TEXT("Tree View"),
        WS_CHILD | WS_VISIBLE | WS_BORDER | WS_SIZEBOX | WS_VSCROLL | WS_TABSTOP | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,
        0,
        0,
        rcClient.right,
        rcClient.bottom,
        hwndParent,
        (HMENU)IDD_TREEVIEW,
        hInst,
        NULL);

    // Initialize the image list, and add items to the control. 
    // InitTreeViewImageLists and InitTreeViewItems are application- 
    // defined functions, shown later. 
    /*if (!InitTreeViewImageLists(hwndTV) ||
        !InitTreeViewItems(hwndTV))
    {
        DestroyWindow(hwndTV);
        return FALSE;
    }*/
    return hwndTV;
}

LPCTSTR OpenDirectory(HWND hwndOwner)
{
    BROWSEINFO bInfo;
    ZeroMemory(&bInfo, sizeof(BROWSEINFO));
    static TCHAR folderBuf[MAX_PATH];

    bInfo.hwndOwner = hwndOwner;
    bInfo.pidlRoot = NULL;
    bInfo.pszDisplayName = folderBuf;
    bInfo.lpszTitle = L"Specify project folder\0";
    bInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_SHAREABLE;

    PIDLIST_ABSOLUTE pidl = SHBrowseForFolder(&bInfo);

    if (!pidl) 
    {
        return NULL;
    }

    SHGetPathFromIDList(pidl, folderBuf);
    return folderBuf;
}

HTREEITEM AddItemToTree(HWND hwndTree, TCHAR* text, HTREEITEM hParent)
{
    TVITEM tvi;
    TVINSERTSTRUCT tvins;
    HTREEITEM hti;

    //LPTSTR baseName = const_cast<LPTSTR>(std::filesystem::path(text).filename().wstring().c_str());
    tvi.pszText = text;
    tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
    tvi.cchTextMax = sizeof(tvi.pszText) / sizeof(tvi.pszText[0]);

    tvins.item = tvi;

    tvins.hInsertAfter = hParent;
    tvins.hParent = hParent;
    HTREEITEM hPrev = (HTREEITEM) SendMessage(hwndTv, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);

    if (hPrev == NULL)
    {
        return NULL;
    }

    return hPrev;
}

BOOL ListDirectoryContents(const TCHAR* sDir, HTREEITEM parent)
{
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = 0;
    std::vector<std::wstring> FileNames;

    TCHAR sPath[2048];
    _stprintf_s(sPath, TEXT("%s\\*.*"), sDir);

    if ((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
    {
        
        return FALSE;
    }
    do
    {
        if (_tcscmp(fdFile.cFileName, TEXT(".")) != 0 && _tcscmp(fdFile.cFileName, TEXT("..")) != 0)
        {
            _stprintf_s(sPath, TEXT("%s\\%s"), sDir, fdFile.cFileName);
            if ((fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                if ((fdFile.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == 0)
                {
                    HTREEITEM localParent;
                    // Directories
                    localParent = AddItemToTree(hwndTv, fdFile.cFileName, parent);
                    //FileNames.insert(sPath);
                    ListDirectoryContents(sPath, localParent); // Recursion
                }
            }
            else
            {
                if ((fdFile.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == 0)
                {
                    //Files
                    AddItemToTree(hwndTv, fdFile.cFileName, parent);
                    FileNames.push_back(sPath);
                }
            }
        }
    } while (FindNextFile(hFind, &fdFile));

    FindClose(hFind);

    for (auto FileName = FileNames.begin(); FileName != FileNames.end(); ++FileName)
    {
        //AddItemToTree(hwndTv, fdFile.cFileName, parent);
    }

    return true;
}

std::vector<WIN32_FIND_DATAW> FindDirEntries(LPCTSTR dirFullPath)
{
    WIN32_FIND_DATAW ffd;
    ZeroMemory(&ffd, sizeof(WIN32_FIND_DATAW));
    TCHAR szDir[MAX_PATH];
    HANDLE hFind = INVALID_HANDLE_VALUE;
    std::vector<WIN32_FIND_DATAW> result;

    // Prepare string for use with FindFile functions.  First, copy the
    // string to a buffer, then append '\*' to the directory name.

    StringCchCopy(szDir, MAX_PATH, dirFullPath);
    StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

    // Find the first file in the directory.
    hFind = FindFirstFile(szDir, &ffd);

    // List all the files in the directory

    do
    {
        if (!(_tcscmp(ffd.cFileName, L".") == 0 || _tcscmp(ffd.cFileName, L"..") == 0))
        {
            result.push_back(ffd);
        }
    } while (FindNextFile(hFind, &ffd) != 0);

    FindClose(hFind);
    return result;
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
        RECT rect;
        GetClientRect(hWnd, &rect);
        //CreateRichEdit(hWnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, hInst);
        hwndTv = CreateATreeView(hWnd);
    }
    break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDM_OPEN_PROJECT:
            {
                LPCTSTR path = OpenDirectory(hWnd);
                if (path == NULL)
                {
                    break;
                }
                if (treeRoot != NULL)
                {
                    TreeView_DeleteAllItems(hwndTv);
                    treeRoot = NULL;
                }
                treeRoot = AddItemToTree(hwndTv, const_cast<LPTSTR>(path), TVI_ROOT);
                ListDirectoryContents(path, treeRoot);
                //std::vector<WIN32_FIND_DATA> entrs = FindDirEntries(path);
                break;
            }
            case IDM_CLOSE_PROJECT:
            {
                if (treeRoot != NULL)
                {
                    TreeView_DeleteAllItems(hwndTv);
                    treeRoot = NULL;
                }
            }
            break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
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

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
