#include "wndproc.h"

// Message handler for create file box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK CreateFileDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        SetWindowLongPtr(hDlg, GWLP_USERDATA, (LONG)lParam);
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            TCHAR* newFileName = GetDialogInput(hDlg);

            LPCFDATA cfData = (LPCFDATA)GetWindowLongPtr(hDlg, GWLP_USERDATA);

            BOOL added = AddProjectItem(cfData->parentPath, newFileName, cfData->createType);

            size_t newFileLength = sizeof(cfData->parentPath) + sizeof(newFileName) + sizeof(_T("\\")) + 1;
            TCHAR* newFilePath = new TCHAR[newFileLength];
            newFilePath[0] = _T('\0');

            _tcscat_s(newFilePath, newFileLength - _tcslen(newFilePath), cfData->parentPath);
            _tcscat_s(newFilePath, newFileLength - _tcslen(newFilePath), _T("\\"));
            _tcscat_s(newFilePath, newFileLength - _tcslen(newFilePath), newFileName);

            LPFINFO fInfo = CreateProjectFileInfo(newFileName, newFilePath, cfData->createType);
            
            AddItemToTree(cfData->hwndTv, fInfo, cfData->parentNode);

            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK RenameFileDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        SetWindowLongPtr(hDlg, GWLP_USERDATA, (LONG)lParam);
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            TCHAR* newFileName = GetDialogInput(hDlg);

            LPRFDATA rfData = (LPRFDATA)GetWindowLongPtr(hDlg, GWLP_USERDATA);

            BOOL renamed = RenameProjectFile(rfData->oldFilePath, newFileName);

            if (renamed)
            {
                TVITEM item;
                item.hItem = rfData->itemToRename;

                item.mask = TVIF_PARAM;
                TreeView_GetItem(rfData->hwndTv, &item);

                LPFINFO fInfo = (LPFINFO)item.lParam;

                fInfo->fileFullPath = ReplaceFileName(rfData->oldFilePath, newFileName);
                fInfo->fileName = newFileName;

                item.mask = TVIF_TEXT | TVIF_PARAM;
                item.pszText = newFileName;
                item.cchTextMax = _tcslen(newFileName);
                item.lParam = (LPARAM)fInfo;

                TreeView_SetItem(rfData->hwndTv, &item);
            }

            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK CreateProjectDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        SetWindowLongPtr(hDlg, GWLP_USERDATA, lParam);
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            TCHAR* newProjectName = GetDialogInput(hDlg);



            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDC_SELECTPROJECTFOLD)
        {

            LPCTSTR path = OpenDirectory(hDlg);

            WORD length = (WORD)SendDlgItemMessage(hDlg,
                IDC_NEWPROJECTFOLDER,
                EM_LINELENGTH,
                (WPARAM)0,
                (LPARAM)0);

            SendDlgItemMessage(hDlg,
                IDC_NEWPROJECTFOLDER,
                EM_SETSEL,
                (WPARAM)0,
                (LPARAM)-1);

            SendDlgItemMessage(hDlg,
                IDC_NEWPROJECTFOLDER,
                EM_REPLACESEL,
                (WPARAM)FALSE,
                (LPARAM)path);

            HWND hwndTv = (HWND)GetWindowLongPtr(hDlg, GWLP_USERDATA);

            LPFINFO fInfo = new FINFO;

            fInfo->fileFullPath = const_cast<TCHAR*>(path);
            fInfo->fileName = const_cast<TCHAR*>(path);
            fInfo->fType = FileType::PROOT;

            HTREEITEM hti = AddItemToTree(hwndTv, fInfo, TVI_ROOT);

            ListDirectoryContents(hwndTv, path, hti);

            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}