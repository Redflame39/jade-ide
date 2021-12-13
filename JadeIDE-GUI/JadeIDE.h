#pragma once

#include "resource.h"
#include "wndproc.h"
#include "structures.h"
#include <CommCtrl.h>
#include "treeview.h"
#include "redit.h"

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);