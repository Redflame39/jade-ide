// JadeIDE-GUI.cpp : Defines the entry point for the application.
//
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "Shlwapi.lib")

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
#include <windowsx.h>
#include "FilesFunctions.h"

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
                    LPFINFO pInfo = CreateProjectFileInfo(fdFile.cFileName, sPath, FileType::PDIRECTORY);
                    // Directories
                    localParent = AddItemToTree(hwndTv, pInfo, parent);
                    //FileNames.insert(sPath);
                    ListDirectoryContents(sPath, localParent); // Recursion
                }
            }
            else
            {
                if ((fdFile.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == 0)
                {
                    //Files
                    LPFINFO pInfo = CreateProjectFileInfo(fdFile.cFileName, sPath, FileType::PFILE);
                    AddItemToTree(hwndTv, pInfo, parent);
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
        CreateRichEdit(hWnd, hInst);
        hwndTv = CreateATreeView(hInst, hWnd);
    }
    break;
    case WM_CONTEXTMENU:
    {
        RECT rcTree;
        HTREEITEM htvItem;
        TVHITTESTINFO htInfo = { 0 };

        GetWindowRect(hwndTv, &rcTree);

        long xPos = GET_X_LPARAM(lParam) - rcTree.left;   // x position from message, in screen coordinates
        long yPos = GET_Y_LPARAM(lParam) - rcTree.top - 1;   // y position from message, in screen coordinates 

                      // get its window coordinates
        htInfo.pt.x = xPos;            // convert to client coordinates
        htInfo.pt.y = yPos;

        if (htvItem = TreeView_HitTest(hwndTv, &htInfo)) {    // hit test
            TreeView_SelectItem(hwndTv, htvItem);           // success; select the item
            TVITEM item;
            TCHAR buf[100];
            item.hItem = htvItem;
            item.mask = TVIF_PARAM;
            TreeView_GetItem(hwndTv, &item);

            HMENU hMenu;
            HMENU hmenuTrackPopup;

            hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDC_TREEVIEWCONTEXT));
            if (hMenu == NULL)
            {
                break;
            }

            hmenuTrackPopup = GetSubMenu(hMenu, 0);
            POINT pt;
            pt.x = xPos;
            pt.y = yPos;
            ClientToScreen(hWnd, &pt);

            LPFINFO fInfo = (LPFINFO)item.lParam;

            if (fInfo->fType == FileType::PFILE)
            {
                EnableMenuItem(hmenuTrackPopup, IDM_CONTEXTCREATEFILE, MF_GRAYED);
                EnableMenuItem(hmenuTrackPopup, IDM_CONTEXTCREATEPACKAGE, MF_GRAYED);
            }
            TrackPopupMenu(hmenuTrackPopup, TPM_LEFTALIGN | TPM_LEFTBUTTON,
                pt.x, pt.y, 0, hWnd, NULL);
        }
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
            case IDM_CREATE_PROJECT:
            {

            }
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
                LPFINFO pInfo = CreateProjectFileInfo(const_cast<LPTSTR>(path), const_cast<LPTSTR>(path), FileType::PROOT);
                treeRoot = AddItemToTree(hwndTv, pInfo, TVI_ROOT);
                ListDirectoryContents(path, treeRoot);
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
            case IDM_CONTEXTCREATEFILE:
            {
                HTREEITEM htvItem;
                TVITEM item;

                htvItem = TreeView_GetSelection(hwndTv);
                item.hItem = htvItem;
                item.mask = TVIF_PARAM;
                TreeView_GetItem(hwndTv, &item);

                LPFINFO fInfo = (LPFINFO)item.lParam;

                LPCFDATA cfData = CreateCFDATA(fInfo->fileFullPath, htvItem);
                cfData->hwndTv = hwndTv;
                cfData->createType = FileType::PFILE;
                DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_CREATEFILEBOX), hWnd, CreateFileDialog, (LPARAM)cfData);
            }
            break;
            case IDM_CONTEXTCREATEPACKAGE:
            {
                HTREEITEM htvItem;
                TVITEM item;

                htvItem = TreeView_GetSelection(hwndTv);
                item.hItem = htvItem;
                item.mask = TVIF_PARAM;
                TreeView_GetItem(hwndTv, &item);

                LPFINFO fInfo = (LPFINFO)item.lParam;

                LPCFDATA cfData = CreateCFDATA(fInfo->fileFullPath, htvItem);
                cfData->hwndTv = hwndTv;
                cfData->createType = FileType::PDIRECTORY;
                DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_CREATEFILEBOX), hWnd, CreateFileDialog, (LPARAM)cfData);
            }
            break;
            case IDM_CONTEXTOPEN:
            {
                MessageBox(hWnd, L"", L"", MB_OK);
            }
            break;
            case IDM_CONTEXTDELETE:
            {
                HTREEITEM htvItem;
                TVITEM item;

                htvItem = TreeView_GetSelection(hwndTv);
                item.hItem = htvItem;
                item.mask = TVIF_PARAM;
                TreeView_GetItem(hwndTv, &item);

                LPFINFO fInfo = (LPFINFO)item.lParam;

                switch (fInfo->fType)
                {
                case FileType::PFILE:
                {
                    
                    int answ = MessageBox(hWnd, L"Are you sure you want to delete this file?", L"Deleting file", MB_YESNO);
                    if (answ == IDYES)
                    {
                        BOOL deleted = DeleteProjectFile(fInfo->fileFullPath);
                        if (deleted)
                        {
                            TreeView_DeleteItem(hwndTv, htvItem);
                            MessageBox(hWnd, L"File was successfully deleted", L"Successfully deleted", MB_OK);
                        }
                    }
                }
                break;
                case FileType::PDIRECTORY:
                {
                    if (PathIsDirectoryEmpty(fInfo->fileFullPath))
                    {
                        if (RemoveDirectory(fInfo->fileFullPath))
                        {
                            TreeView_DeleteItem(hwndTv, htvItem);
                            MessageBox(hWnd, L"Package was successfully deleted", L"Successfully deleted", MB_OK);
                        }
                    }
                    else
                    {
                        int answ = MessageBox(hWnd, L"Are you sure you want to delete this package?", L"Deleting package", MB_YESNO);
                        if (answ == IDYES)
                        {
                            if (DeleteNonEmptyPackage(fInfo->fileFullPath))
                            {
                                TreeView_DeleteItem(hwndTv, htvItem);
                                MessageBox(hWnd, L"Package was successfully deleted", L"Successfully deleted", MB_OK);
                            }
                        }
                    }
                }
                break;
                default:
                    break;
                }
            }
            break;
            case IDM_CONTEXTRENAME:
            {
                HTREEITEM htvItem;
                TVITEM item;

                htvItem = TreeView_GetSelection(hwndTv);
                item.hItem = htvItem;
                item.mask = TVIF_PARAM;
                TreeView_GetItem(hwndTv, &item);

                LPFINFO fInfo = (LPFINFO)item.lParam;

                LPRFDATA rfData = CreateRFDATA(fInfo->fileFullPath, hwndTv, htvItem);
                DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_RENAMEFILEBOX), hWnd, RenameFileDialog, (LPARAM)rfData);
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
