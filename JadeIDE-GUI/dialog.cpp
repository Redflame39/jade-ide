#include "dialog.h"

TCHAR* GetDialogInput(HWND hDlg, long inputId)
{

    WORD length = (WORD)SendDlgItemMessage(hDlg,
        inputId,
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

    TCHAR* dialogText = new TCHAR[length];

    // Get the characters. 
    SendDlgItemMessage(hDlg,
        inputId,
        EM_GETLINE,
        (WPARAM)0,       // line 0 
        (LPARAM)dialogText);

    // Null-terminate the string. 
    dialogText[length] = 0;

    return dialogText;
}