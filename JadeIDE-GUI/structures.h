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

typedef struct ProjectFileInfo {
    TCHAR* fileFullPath;
    TCHAR* fileName;
    FileType fType;
} FINFO, * LPFINFO;

typedef struct CreateFileData {
    TCHAR* parentPath;
    HTREEITEM parentNode;
    HWND hwndTv;
    FileType createType;
} CFDATA, * LPCFDATA;

LPFINFO CreateProjectFileInfo(TCHAR* fileName, TCHAR* fullPath, FileType fType);

LPCFDATA CreateCFDATA(TCHAR* parentPath, HTREEITEM parentNode);