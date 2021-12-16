#pragma once

#include <Richedit.h>

HWND CreateRichEdit(HWND hwndOwner,        // Dialog box handle.
    HINSTANCE hinst);       // Application or DLL instance.

BOOL SetTextToREdit(HWND hWnd, HWND hwndRedit, TCHAR* text);

TCHAR* GetREditText(HWND hWnd, HWND hwndRedit);
