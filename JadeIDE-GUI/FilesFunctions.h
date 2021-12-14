#pragma once

#include <Windows.h>
#include "JadeIDE.h"
#include <shellapi.h>

BOOL AddProjectItem(const TCHAR* parent, const TCHAR* fileName, FileType fType);

BOOL DeleteProjectFile(TCHAR* fullPath);

BOOL DeleteEmptyPackage(TCHAR* fullPath);

BOOL DeleteNonEmptyPackage(TCHAR* fullPath);

BOOL RenameProjectFile(TCHAR* oldFilePath, TCHAR* newFileName);

TCHAR* ReplaceFileName(TCHAR* oldFilePath, TCHAR* newFileName);