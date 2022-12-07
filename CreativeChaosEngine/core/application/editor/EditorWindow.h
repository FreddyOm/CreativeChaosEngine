#pragma once

#include <Windows.h>
#include <string>
#if _DEBUG
#include "../../../test/editor_test.h"
#endif // unit testing

class EditorWindow
{
#if _DEBUG
	friend struct EditorTest;
#endif // unit testing
public:
#if _DEBUG
	EditorWindow();
#endif // unit testing

	EditorWindow(HINSTANCE hInstance, wchar_t* windowName);
	~EditorWindow();

private:
	wchar_t* CLASS_NAME = L"Default Window Name";
	HINSTANCE* hInst = nullptr;
	WNDCLASS wc = { };
	HWND hWnd = { 0 };

	unsigned int win_width = 1280;
	unsigned int win_height = 720;

public:
	HWND GetWindowHandle();
	WNDCLASS GetWindowClass();
	wchar_t* GetWindowClassName();
	unsigned int GetWindowHeight();
	unsigned int GetWindowWidth();

	void SetWindowHandle(HWND& handle);
	void SetWindowClass(WNDCLASS& wndClass);
	void SetWindowClassName(wchar_t* wndClassName);
	void SetWindowHeight(const unsigned int height);
	void SetWindowWidth(const unsigned int width);

	int HandleMessages();
};
