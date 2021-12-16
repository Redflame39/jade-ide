#pragma once

#include <Windows.h>
#include "JadeIDE.h"
#include <shellapi.h>
#include <shlobj_core.h>
#include <string>
#include <vector>

BOOL AddProjectItem(const TCHAR* parent, const TCHAR* fileName, FileType fType);

BOOL DeleteProjectFile(TCHAR* fullPath);

BOOL DeleteEmptyPackage(TCHAR* fullPath);

BOOL DeleteNonEmptyPackage(TCHAR* fullPath);

BOOL RenameProjectFile(TCHAR* oldFilePath, TCHAR* newFileName);

TCHAR* ReplaceFileName(TCHAR* oldFilePath, TCHAR* newFileName);

TCHAR* AppendFileName(TCHAR* baseFilePath, TCHAR* fileName);

BOOL IsPackageExists(TCHAR* path);

LPCTSTR OpenDirectory(HWND hwndOwner);

BOOL ListDirectoryContents(HWND hwndTv, const TCHAR* sDir, HTREEITEM parent);

LPFINFO GetSelectedProjectFileInfo(HWND hwndTv);

BOOL CreateMainPropertyFile(LPFINFO fInfo);

BYTE* ReadFileData(TCHAR* filePath);

BOOL WriteToFile(TCHAR* filePath, TCHAR* toWrite);