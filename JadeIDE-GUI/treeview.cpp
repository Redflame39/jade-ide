#include "treeview.h"

// Create a tree-view control. 
// Returns the handle to the new control if successful,
// or NULL otherwise. 
// hwndParent - handle to the control's parent window. 
// lpszFileName - name of the file to parse for tree-view items.
// g_hInst - the global instance handle.
// ID_TREEVIEW - the resource ID of the control.

HWND CreateATreeView(HINSTANCE hInst, HWND hwndParent)
{
    RECT rcClient;  // dimensions of client area 
    HWND hwndTV;    // handle to tree-view control 

    // Ensure that the common control DLL is loaded. 
    INITCOMMONCONTROLSEX config;
    config.dwSize = sizeof(INITCOMMONCONTROLSEX);
    config.dwICC = ICC_TREEVIEW_CLASSES;
    InitCommonControlsEx(&config);

    // Get the dimensions of the parent window's client area, and create 
    // the tree-view control. 
    GetClientRect(hwndParent, &rcClient);
    hwndTV = CreateWindowEx(0,
        WC_TREEVIEW,
        TEXT("Tree View"),
        WS_CHILD | WS_VISIBLE | WS_BORDER | WS_SIZEBOX | WS_VSCROLL | WS_TABSTOP | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,
        0,
        0,
        rcClient.right * 0.25,
        rcClient.bottom,
        hwndParent,
        (HMENU)IDD_TREEVIEW,
        hInst,
        NULL);

    // Initialize the image list, and add items to the control. 
    // InitTreeViewImageLists and InitTreeViewItems are application- 
    // defined functions, shown later. 
    if (!InitTreeViewImageLists(hInst, hwndTV))
    {
        DestroyWindow(hwndTV);
        return FALSE;
    }
    return hwndTV;
}

HTREEITEM AddItemToTree(HWND hwndTree, LPFINFO fInfo, HTREEITEM hParent)
{
    TVITEM tvi;
    TVINSERTSTRUCT tvins;
    HTREEITEM hti;
    LPTIDATA lptiData = (LPTIDATA)GetWindowLongPtr(hwndTree, GWLP_USERDATA);

    //LPTSTR baseName = const_cast<LPTSTR>(std::filesystem::path(text).filename().wstring().c_str());
    tvi.pszText = fInfo->fileName;
    tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
    tvi.cchTextMax = sizeof(tvi.pszText) / sizeof(tvi.pszText[0]);
    tvi.lParam = (LPARAM)fInfo;

    switch (fInfo->fType)
    {
    case FileType::PROOT:
    {
        tvi.iImage = lptiData->rootIconId;
        tvi.iSelectedImage = lptiData->rootIconId;
    }
    break;
    case FileType::PFILE:
    {
        tvi.iImage = lptiData->fileIconId;
        tvi.iSelectedImage = lptiData->fileIconId;
    }
    break;
    case FileType::PDIRECTORY:
    {
        tvi.iImage = lptiData->packageIconId;
        tvi.iSelectedImage = lptiData->packageIconId;
    }
    break;
    default:
        break;
    }

    tvins.item = tvi;

    tvins.hInsertAfter = hParent;
    tvins.hParent = hParent;
    HTREEITEM hPrev = (HTREEITEM)SendMessage(hwndTree, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);

    if (hPrev == NULL)
    {
        return NULL;
    }

    return hPrev;
}

// InitTreeViewImageLists - creates an image list, adds three bitmaps 
// to it, and associates the image list with a tree-view control. 
// Returns TRUE if successful, or FALSE otherwise. 
// hwndTV - handle to the tree-view control. 
//
// Global variables and constants: 
// g_hInst - the global instance handle.
// g_nOpen, g_nClosed, and g_nDocument - global indexes of the images. 
// CX_BITMAP and CY_BITMAP - width and height of an icon. 
// NUM_BITMAPS - number of bitmaps to add to the image list. 
// IDB_OPEN_FILE, IDB_CLOSED_FILE, IDB_DOCUMENT -
//     resource identifiers of the bitmaps.

BOOL InitTreeViewImageLists(HINSTANCE hInst, HWND hwndTV)
{
    HIMAGELIST himl;  // handle to image list 
    HBITMAP hbmp;     // handle to bitmap 
    LPTIDATA lptiData = new TIDATA;

    // Create the image list. 
    if ((himl = ImageList_Create(30,
        30,
        FALSE,
        3, 0)) == NULL)
        return FALSE;

    // Add the open file, closed file, and document bitmaps. 
    hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FILETREEICO));
    lptiData->fileIconId = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
    DeleteObject(hbmp);

    hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FOLDERTREEICO));
    lptiData->packageIconId = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
    DeleteObject(hbmp);

    hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ROOTTREEICO));
    lptiData->rootIconId = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
    DeleteObject(hbmp);

    // Fail if not all of the images were added. 
    if (ImageList_GetImageCount(himl) < 3)
        return FALSE;

    // Associate the image list with the tree-view control. 
    TreeView_SetImageList(hwndTV, himl, TVSIL_NORMAL);

    SetWindowLongPtr(hwndTV, GWLP_USERDATA, (LONG_PTR)lptiData);

    return TRUE;
}