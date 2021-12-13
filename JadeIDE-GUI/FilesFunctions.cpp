#include "FilesFunctions.h"
#include <tchar.h>

BOOL AddProjectItem(const TCHAR* parent, const TCHAR* fileName, FileType fType)
{
	TCHAR *newFilePath = new TCHAR[2048];

	_stprintf_s(newFilePath, 2048, TEXT("%s\\%s"), parent, fileName);

	switch (fType)
	{
	case PFILE:
	{
		HANDLE hFile = CreateFile(newFilePath, NULL, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile == NULL)
		{
			return FALSE;
		}
		CloseHandle(hFile);
	}
		break;
	case PDIRECTORY:
	{
		BOOL created = CreateDirectory(newFilePath, NULL);
		if (created == NULL)
		{
			return FALSE;
		}
	}
		break;
	default:
		break;
	}
	delete[] newFilePath;
	return TRUE;
}