#include "build.h"

void BuildProject(HWND hwndTv, HTREEITEM src, HTREEITEM root)
{
    GenerateSourcesFile(hwndTv, src);

    TVITEM tvi;
    tvi.hItem = root;
    tvi.mask = TVIF_PARAM;
    TreeView_GetItem(hwndTv, &tvi);
    LPFINFO fInfo = (LPFINFO)tvi.lParam;

    TVITEM tviSrc;
    tviSrc.hItem = src;
    tviSrc.mask = TVIF_PARAM;
    TreeView_GetItem(hwndTv, &tviSrc);
    LPFINFO srcFInfo = (LPFINFO)tviSrc.lParam;

    AddProjectItem(fInfo->fileFullPath, L"build", FileType::PDIRECTORY);

    TCHAR* sourcesFilePath = AppendFileName(srcFInfo->fileFullPath, const_cast<TCHAR*>(L"sources.txt"));

    TCHAR* pathToJdk = ReadFileData(AppendFileName(fInfo->fileFullPath, const_cast<TCHAR*>(JDKPATH)));

    TCHAR* outPath = AppendFileName(fInfo->fileFullPath, const_cast<TCHAR*>(L"build"));

    CompileSource(sourcesFilePath, pathToJdk, outPath, root);

}

HTREEITEM LoadMainClass(HWND hwndTv, HTREEITEM root)
{
    if (root == NULL)
    {
        return NULL;
    }
    TVITEM tvi;
    tvi.hItem = root;
    tvi.mask = TVIF_PARAM;
    TreeView_GetItem(hwndTv, &tvi);
    LPFINFO fInfo = (LPFINFO)tvi.lParam;
    TCHAR* mainPropertyFile = const_cast<TCHAR*>(MAINCLASS);
    TCHAR* mainClass = ReadFileData(AppendFileName(fInfo->fileFullPath, mainPropertyFile));
    HTREEITEM mainItem = FindByFullPath(hwndTv, mainClass);
    if (mainItem != NULL)
    {
        SetMainIcon(hwndTv, mainItem, NULL);
    }
    return mainItem;
}

HTREEITEM LoadSrcPackage(HWND hwndTv, HTREEITEM root)
{
    if (root == NULL)
    {
        return NULL;
    }
    TVITEM tvi;
    tvi.hItem = root;
    tvi.mask = TVIF_PARAM;
    TreeView_GetItem(hwndTv, &tvi);
    LPFINFO fInfo = (LPFINFO)tvi.lParam;
    TCHAR* srcPropertyFile = const_cast<TCHAR*>(SRCPACKAGE);
    TCHAR* srcPkg = ReadFileData(AppendFileName(fInfo->fileFullPath, srcPropertyFile));
    HTREEITEM srcItem = FindByFullPath(hwndTv, srcPkg);
    if (srcItem != NULL)
    {
        MarkPackageAsSource(hwndTv, srcItem, NULL);
    }
    return srcItem;
}

HTREEITEM CreateSrcPackage(HWND hwndTv, HTREEITEM hParent, TCHAR* parentPath)
{
    TCHAR* src = const_cast<TCHAR*>(TEXT("src"));
    TCHAR* srcPath = AppendFileName(parentPath, src);
    if (!IsPackageExists(srcPath))
    {
        AddProjectItem(parentPath, src, FileType::PDIRECTORY);
    }

    LPFINFO fSrcInfo = new FINFO;

    fSrcInfo->fileFullPath = srcPath;
    fSrcInfo->fileName = src;
    fSrcInfo->fType = PDIRECTORY;
    fSrcInfo->isSource = TRUE;

    HTREEITEM hSrc = AddItemToTree(hwndTv, fSrcInfo, hParent);

    MarkPackageAsSource(hwndTv, hSrc, NULL);
    
    return hSrc;
}

BOOL ValidateJDK(TCHAR* pathToJdk)
{
    TCHAR* pathToJavac = AppendFileName(pathToJdk, const_cast<TCHAR*>(L"bin\\javac.exe"));
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    BOOL created = CreateProcess(
        pathToJavac,
        const_cast<TCHAR*>(L"--version"),
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return created;

}

TCHAR* GenerateSourcesFile(HWND hwndTv, HTREEITEM src)
{
    TVITEM tvi;
    tvi.hItem = src;
    tvi.mask = TVIF_PARAM;
    TreeView_GetItem(hwndTv, &tvi);
    LPFINFO fInfo = (LPFINFO)tvi.lParam;

    CreateSourceCollectBatchFile(fInfo->fileFullPath);

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    DWORD error = GetLastError();

    BOOL created = CreateProcess(L"C:\\Windows\\System32\\cmd.exe",
        const_cast<TCHAR*>(L"/c C:\\Users\\vanya\\source\\repos\\ide\\JadeIDE-GUI\\SaveSources.bat"),
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi);

    error = GetLastError();

    if (created)
    {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        TCHAR* pathToSourcesFile = AppendFileName(fInfo->fileFullPath, const_cast<TCHAR*>(L"sources.txt"));

        return PathFileExists(pathToSourcesFile) ? pathToSourcesFile : NULL;
    }
    else
    {
        return FALSE;
    }
}

void CreateSourceCollectBatchFile(TCHAR* pathSrc)
{
    long length = strlen("cd ") + _tcslen(pathSrc) + strlen(SOURCE_COLLECT_COMMAND) + strlen("\n") + 1;
    char* buf = new char[length];
    buf[0] = '\0';
    _bstr_t b(pathSrc);
    StringCchCatA(buf, length, "cd ");
    StringCchCatA(buf, length, b);
    StringCchCatA(buf, length, "\n");
    StringCchCatA(buf, length, SOURCE_COLLECT_COMMAND);

    WriteToFileAnsi(const_cast<char*>("SaveSources.bat"), buf);
}

void CompileSource(TCHAR* sourcesFilePath, TCHAR* jdkPath, TCHAR* outPath, HTREEITEM root)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));


    TCHAR* pathToJavac = AppendFileName(jdkPath, const_cast<TCHAR*>(L"bin\\javac.exe"));
    TCHAR* encoding = const_cast<TCHAR*>(L" -encoding UTF-16LE");

    long size = _tcslen(L"@") + _tcslen(sourcesFilePath) + _tcslen(L" -d ") + _tcslen(outPath) + _tcslen(encoding) + 1;
    TCHAR* cmdLine = new TCHAR[size];
    cmdLine[0] = '\0';
    StringCchCat(cmdLine, size, L"@");
    StringCchCat(cmdLine, size, sourcesFilePath);
    StringCchCat(cmdLine, size, L" -d ");
    StringCchCat(cmdLine, size, outPath);
    StringCchCat(cmdLine, size, encoding);

    BOOL created = CreateProcess(pathToJavac,
        cmdLine,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi);

    if (created)
    {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
}