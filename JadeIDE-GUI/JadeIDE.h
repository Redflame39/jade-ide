#pragma once

#include "resource.h"

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
HWND CreateRichEdit(HWND hwndOwner,        // Dialog box handle.
    int x, int y,          // Location.
    int width, int height, // Dimensions.
    HINSTANCE hinst);       // Application or DLL instance.
HWND CreateATreeView(HWND hwndParent);

struct ProjectFileInfo {
    TCHAR fileFullPath[MAX_PATH];
    TCHAR fileName[MAX_PATH];
};