#pragma once

#include <Windows.h>
#include <tchar.h>
#include "resource.h"
#include "structures.h"
#include "FilesFunctions.h"
#include "dialog.h"
#include "treeview.h"

INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    CreateFileDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    RenameFileDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    CreateProjectDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);