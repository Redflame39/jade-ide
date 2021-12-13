#pragma once

#include <windows.h>
#include <CommCtrl.h>
#include "structures.h"
#include "Resource.h"

HWND CreateATreeView(HINSTANCE hInst, HWND hwndParent);

HTREEITEM AddItemToTree(HWND hwndTree, LPFINFO fInfo, HTREEITEM hParent);

BOOL InitTreeViewImageLists(HINSTANCE hInst, HWND hwndTV);

typedef struct TreeIconsData {
	int rootIconId;
	int fileIconId;
	int packageIconId;
	HIMAGELIST himl;
} TIDATA, *LPTIDATA;
