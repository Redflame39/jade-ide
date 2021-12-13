#pragma once

#include <Windows.h>
#include "JadeIDE.h"

BOOL AddProjectItem(const TCHAR* parent, const TCHAR* fileName, FileType fType);

BOOL DeleteProjectFile(TCHAR* fullPath);

TCHAR* RenameProjectFile(TCHAR* fullPath);