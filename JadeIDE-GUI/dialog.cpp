#include "dialog.h"

TCHAR* GetDialogInput(HWND hDlg)
{
    TCHAR dialogText[256];

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
        (LPARAM)dialogText);

    // Null-terminate the string. 
    dialogText[length] = 0;

    return dialogText;
}