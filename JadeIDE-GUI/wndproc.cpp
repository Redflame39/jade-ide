#include "wndproc.h"

// Message handler for about box.
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
            TCHAR newFileName[256];

            WORD length = (WORD)SendDlgItemMessage(hDlg,
                IDC_NEWFILETEXT,
                EM_LINELENGTH,
                (WPARAM)0,
                (LPARAM)0);

            if (length >= 256)
            {
                MessageBox(hDlg,
                    L"Too many characters.",
                    L"Error",
                    MB_OK);
                EndDialog(hDlg, TRUE);
                return FALSE;
            }

            // Get the characters. 
            SendDlgItemMessage(hDlg,
                IDC_NEWFILETEXT,
                EM_GETLINE,
                (WPARAM)0,       // line 0 
                (LPARAM)newFileName);

            // Null-terminate the string. 
            newFileName[length] = 0;

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