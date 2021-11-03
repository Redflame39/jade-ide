#include "CodeEditorWindow.h"

CodeEditorWindow::CodeEditorWindow(HWND parent)
{
	pHandle = parent;
}

CodeEditorWindow::~CodeEditorWindow()
{

}

BOOL CodeEditorWindow::CreateEditorWindow(RECT cRect)
{
	editorHandle = CreateWindowW(
		L"EDIT",
		nullptr,
		WS_BORDER | WS_CHILD | WS_VISIBLE |
		WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
		cRect.left,         // x position 
		cRect.top,         // y position 
		cRect.right - cRect.left,        // width
		cRect.bottom - cRect.top,        // height
		pHandle,
		nullptr,
		(HINSTANCE)GetWindowLongPtr(pHandle, GWLP_HINSTANCE),
		nullptr);

	return editorHandle != 0;
}

HWND CodeEditorWindow::GetHandle() const
{
	return editorHandle;
}