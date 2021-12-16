#include "build.h"

void LoadMainClass(HWND hwndTv, HTREEITEM root)
{
	TVITEM tvi;
	tvi.hItem = root;
	tvi.mask = TVIF_PARAM;
	TreeView_GetItem(hwndTv, &tvi);
	//todo
}