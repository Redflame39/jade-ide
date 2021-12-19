#pragma once

#include <windows.h>
#include <strsafe.h>
#include <comdef.h>
#include <pathcch.h>
#include "FilesFunctions.h"

void BuildProject(HWND hwndTv, HTREEITEM src, HTREEITEM root);

HTREEITEM LoadMainClass(HWND hwndTv, HTREEITEM root);

HTREEITEM LoadSrcPackage(HWND hwndtv, HTREEITEM root);

HTREEITEM CreateSrcPackage(HWND hwndTv, HTREEITEM hParent, TCHAR* parentPath);

BOOL ValidateJDK(TCHAR* pathToJdk);

TCHAR* GenerateSourcesFile(HWND hwndTv, HTREEITEM root);

void CreateSourceCollectBatchFile(TCHAR* pathSrc);

void CompileSource(TCHAR* sourcesFilePath, TCHAR* jdkPath, TCHAR* outPath, HTREEITEM root);