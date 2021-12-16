#pragma once

#include <windows.h>
#include <tchar.h>
#include <CommCtrl.h>

enum FileType
{
    PFILE,
    PDIRECTORY,
    PROOT
};

typedef struct ControlHandles
{
    HWND hwndTv;
    HWND hwndRedit;
} HCSTRUCT, *LPHCSTRUCT;

typedef struct ProjectFileInfo 
{
    TCHAR* fileFullPath;
    TCHAR* fileName;
    FileType fType;
    BOOL isMain;
    BOOL isSource;
} FINFO, * LPFINFO;

typedef struct CreateFileData 
{
    TCHAR* parentPath;
    HTREEITEM parentNode;
    HWND hwndTv;
    FileType createType;
} CFDATA, * LPCFDATA;

typedef struct RenameFileData 
{
    TCHAR* oldFilePath;
    HWND hwndTv;
    HTREEITEM itemToRename;
} RFDATA, *LPRFDATA;

LPFINFO CreateProjectFileInfo(TCHAR* fileName, TCHAR* fullPath, FileType fType);

LPCFDATA CreateCFDATA(TCHAR* parentPath, HTREEITEM parentNode);

LPRFDATA CreateRFDATA(TCHAR* oldFilePath, HWND hwndTv, HTREEITEM hti);