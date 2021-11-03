#pragma once
#include <Windows.h>

class CodeEditorWindow
{
public:
	CodeEditorWindow(HWND parent);
	~CodeEditorWindow();

	//Create window instance
	BOOL CreateEditorWindow(RECT cRect);

	//Show window
	void Show(int cmdShow);

	HWND GetHandle() const;

private:
	HWND pHandle;
	HWND editorHandle;
};