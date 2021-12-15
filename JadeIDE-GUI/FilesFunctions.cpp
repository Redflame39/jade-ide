#include "FilesFunctions.h"
#include <tchar.h>

BOOL AddProjectItem(const TCHAR* parent, const TCHAR* fileName, FileType fType)
{
	TCHAR *newFilePath = new TCHAR[2048];
	BOOL created;

	_stprintf_s(newFilePath, 2048, TEXT("%s\\%s"), parent, fileName);

	switch (fType)
	{
	case PFILE:
	{
		HANDLE hFile = CreateFile(newFilePath, NULL, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile == NULL)
		{
			created = FALSE;
		}
		CloseHandle(hFile);
	}
		break;
	case PDIRECTORY:
	{
		created = CreateDirectory(newFilePath, NULL);
	}
		break;
	default:
		created = FALSE;
		break;
	}
	delete[] newFilePath;
	return created;
}

BOOL RenameProjectFile(TCHAR* oldFilePath, TCHAR* newFileName)
{
	TCHAR* newFilePath = ReplaceFileName(oldFilePath, newFileName);

	BOOL renamed = MoveFile(oldFilePath, newFilePath);

	delete[] newFilePath;
	
	return renamed;
}

TCHAR* ReplaceFileName(TCHAR* oldFilePath, TCHAR* newFileName)
{
	size_t bufLength = _tcslen(oldFilePath) + _tcslen(newFileName) + 1;
	TCHAR* newFilePath = new TCHAR[bufLength];
	newFilePath[0] = _T('\0');

	_tcscat_s(newFilePath, bufLength, oldFilePath);

	int i;
	for (i = _tcslen(newFilePath); i > 0; i--)
	{
		if (newFilePath[i] == _T('\\') || newFilePath[i] == _T('/'))
		{
			newFilePath[i + 1] = _T('\0');
			break;
		}
	}

	_tcscat_s(newFilePath, bufLength, newFileName);

	return newFilePath;
}

BOOL DeleteProjectFile(TCHAR* fullPath)
{
	return DeleteFile(fullPath);
}

BOOL DeleteNonEmptyPackage(TCHAR* fullPath)
{
	size_t length = _tcslen(fullPath);
	TCHAR* tempFullPath = new TCHAR[length + 2];

	tempFullPath[0] = _T('\0');

	_tcscat_s(tempFullPath, length+2, fullPath);
	tempFullPath[length] = _T('\0');
	tempFullPath[length + 1] = _T('\0');

	SHFILEOPSTRUCT shfops;
	shfops.wFunc = FO_DELETE;
	shfops.pFrom = tempFullPath;
	shfops.fFlags = FOF_NO_UI;

	int result = SHFileOperation(&shfops);

	delete[] tempFullPath;
	return result == 0 ? TRUE : FALSE;
}


LPCTSTR OpenDirectory(HWND hwndOwner)
{
	BROWSEINFO bInfo;
	ZeroMemory(&bInfo, sizeof(BROWSEINFO));
	static TCHAR folderBuf[MAX_PATH];

	bInfo.hwndOwner = hwndOwner;
	bInfo.pidlRoot = NULL;
	bInfo.pszDisplayName = folderBuf;
	bInfo.lpszTitle = L"Specify project folder\0";
	bInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_SHAREABLE;

	PIDLIST_ABSOLUTE pidl = SHBrowseForFolder(&bInfo);

	if (!pidl)
	{
		return NULL;
	}

	SHGetPathFromIDList(pidl, folderBuf);
	return folderBuf;
}

BOOL ListDirectoryContents(HWND hwndTv, const TCHAR* sDir, HTREEITEM parent)
{
	WIN32_FIND_DATA fdFile;
	HANDLE hFind = 0;
	std::vector<std::wstring> FileNames;

	TCHAR sPath[2048];
	_stprintf_s(sPath, TEXT("%s\\*.*"), sDir);

	if ((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	do
	{
		if (_tcscmp(fdFile.cFileName, TEXT(".")) != 0 && _tcscmp(fdFile.cFileName, TEXT("..")) != 0)
		{
			_stprintf_s(sPath, TEXT("%s\\%s"), sDir, fdFile.cFileName);
			if ((fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				if ((fdFile.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == 0)
				{
					HTREEITEM localParent;
					LPFINFO pInfo = CreateProjectFileInfo(fdFile.cFileName, sPath, FileType::PDIRECTORY);
					// Directories
					localParent = AddItemToTree(hwndTv, pInfo, parent);
					//FileNames.insert(sPath);
					ListDirectoryContents(hwndTv, sPath, localParent); // Recursion
				}
			}
			else
			{
				if ((fdFile.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == 0)
				{
					//Files
					LPFINFO pInfo = CreateProjectFileInfo(fdFile.cFileName, sPath, FileType::PFILE);
					AddItemToTree(hwndTv, pInfo, parent);
					FileNames.push_back(sPath);	
				}
			}
		}
	} while (FindNextFile(hFind, &fdFile));

	FindClose(hFind);

	for (auto FileName = FileNames.begin(); FileName != FileNames.end(); ++FileName)
	{
		//AddItemToTree(hwndTv, fdFile.cFileName, parent);
	}

	return true;
}

LPFINFO GetSelectedProjectFileInfo(HWND hwndTv)
{
	HTREEITEM hti = TreeView_GetSelection(hwndTv);
	TVITEM item;
	item.hItem = hti;
	item.mask = TVIF_PARAM;
	TreeView_GetItem(hwndTv, &item);
	LPFINFO fInfo = (LPFINFO)item.lParam;
	return fInfo;
}