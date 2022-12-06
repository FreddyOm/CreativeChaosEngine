#pragma once

#define UNICODE
#define _UNICODE

#include <Windows.h>
#include <string>


class EditorWindow
{
public:
	EditorWindow(HINSTANCE hInstance, wchar_t* windowName);
	~EditorWindow();

private:
	wchar_t* CLASS_NAME = L"Default Window Name";
	WNDCLASS wc = { };
	HWND hWnd = { 0 };

	int win_width = 1280;
	int win_height = 720;

private:
	void HandleMessages();
	

public:
	HWND GetWindowHandle();
	WNDCLASS GetWindowClass();
	wchar_t* GetWindowClassName();

	void SetWindowHandle(HWND& handle);
	void SetWindowClass(WNDCLASS& wndClass);
	void SetWindowClassName(wchar_t* wndClassName);

	//LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};