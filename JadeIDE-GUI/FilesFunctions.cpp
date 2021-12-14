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