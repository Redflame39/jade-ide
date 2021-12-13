#pragma once

#include <Windows.h>
#include <tchar.h>
#include "resource.h"
#include "structures.h"
#include "FilesFunctions.h"

INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    CreateFileDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);