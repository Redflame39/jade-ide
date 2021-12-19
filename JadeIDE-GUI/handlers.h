#pragma once

#include "framework.h"
#include <CommCtrl.h>
#include "structures.h"
#include "treeview.h"
#include "redit.h"
#include "FilesFunctions.h"
#include <windowsx.h>

LPHCSTRUCT OnCreate(HINSTANCE hInst, HWND hWnd);

LPFINFO OnFileOpen(HWND hWnd, HWND hwndRedit, HWND hwndTv);

void OnContextMenu(HINSTANCE hInst, HWND hWnd, HWND hwndTv, LPARAM lParam);

void OnContextCreateFile(HINSTANCE hInst, HWND hWnd, HWND hwndTv);

void OnContextCreatePackage(HINSTANCE hInst, HWND hWnd, HWND hwndTv);

void OnContextDelete(HINSTANCE hInst, HWND hWnd, HWND hwndTv);

void OnContextRename(HINSTANCE hInst, HWND hWnd, HWND hwndTv);

HTREEITEM OnContextMarkAsMain(HINSTANCE hInst, HWND hWnd, HWND hwndTv, HTREEITEM htiOld);

HTREEITEM OnContextMarkAsSrc(HINSTANCE hInst, HWND hWnd, HWND hwndTv, HTREEITEM htiOld);

void OnSaveFile(LPFINFO lpCurrentFile, HWND hWnd, HWND hwndRedit);

HTREEITEM OnCreateProject(HINSTANCE hInst, HWND hWnd, HWND hwndTv);

HTREEITEM OnOpenProject(HWND hWnd, HWND hwndTv, HTREEITEM treeRoot);

HTREEITEM OnCloseProject(HWND hwndTv, HTREEITEM treeRoot);

void OnSelectJDK(HWND hWnd, HWND hwndTv, HTREEITEM root);

BOOL OnBuildRun(HWND hwndTv, HTREEITEM root, HTREEITEM src);

void OnSize(HWND hWnd, HWND hwndTv, HWND hwndRedit);

void OnExit(HWND hWnd);

void OnPaint(HWND hWnd);
