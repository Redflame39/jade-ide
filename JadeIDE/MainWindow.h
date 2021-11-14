#pragma once
#include <Windows.h>

class MainWindow
{
public:
	MainWindow();
	~MainWindow();

	static ATOM RegisterWindowClass();

	BOOL CreateMainWindow();

	void Show(int cmdShow);

	BOOL Create(HINSTANCE hInst);

	HWND GetHandle();

	HACCEL GetAccelTable();

protected:

	void OnCreate();

	void OnSize();

private:
	HINSTANCE hInst;
	HWND handle;
	HACCEL hAccelTable;
	static LPCWSTR className;
	static LPCWSTR title;
	static LRESULT __stdcall MainWndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR __stdcall About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};
