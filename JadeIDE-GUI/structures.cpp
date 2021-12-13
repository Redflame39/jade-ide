#include "structures.h"

LPFINFO CreateProjectFileInfo(TCHAR* fileName, TCHAR* fullPath, FileType fType)
{
    LPFINFO pInfo = new FINFO;

    size_t fileNameLength = _tcslen(fileName) + 1;
    pInfo->fileName = new TCHAR[fileNameLength];
    _tcscpy_s(pInfo->fileName, fileNameLength, fileName);

    size_t fullPathLength = _tcslen(fullPath) + 1;
    pInfo->fileFullPath = new TCHAR[fullPathLength];
    _tcscpy_s(pInfo->fileFullPath, fullPathLength, fullPath);

    pInfo->fType = fType;
    return pInfo;
}

LPCFDATA CreateCFDATA(TCHAR* parentPath, HTREEITEM parentNode)
{
    LPCFDATA cfData = new CFDATA;

    cfData->parentPath = parentPath;
    cfData->parentNode = parentNode;

    return cfData;
}