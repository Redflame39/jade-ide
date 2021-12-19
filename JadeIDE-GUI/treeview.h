#pragma once

#include <windows.h>
#include <CommCtrl.h>
#include "structures.h"
#include "Resource.h"

HWND CreateATreeView(HINSTANCE hInst, HWND hwndParent);

HTREEITEM AddItemToTree(HWND hwndTree, LPFINFO fInfo, HTREEITEM hParent);

BOOL InitTreeViewImageLists(HINSTANCE hInst, HWND hwndTV);

void MarkPackageAsSource(HWND hwndTv, HTREEITEM hti, HTREEITEM htiOld);

HTREEITEM FindByFullPath(HWND hwndTv, TCHAR* fullPath);

HTREEITEM FindTreeItem(HWND hwndTv, HTREEITEM hParent, TCHAR* fullPath);

void SetMainIcon(HWND hwndTv, HTREEITEM hti, HTREEITEM htiOld);

void SetSrcIcon(HWND hwndTv, HTREEITEM hti, HTREEITEM htiOld);

typedef struct TreeIconsData {
	int rootIconId;
	int fileIconId;
	int packageIconId;
	int mainIconId;
	int srcIconId;
	HIMAGELIST himl;
} TIDATA, *LPTIDATA;
