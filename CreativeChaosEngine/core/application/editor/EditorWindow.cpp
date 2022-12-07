#include "EditorWindow.h"
#include "../../debug/Debug.h"
#include <iostream>


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

EditorWindow::EditorWindow(HINSTANCE hInstance, wchar_t* windowName)
{
	// set window class name
	SetWindowClassName(windowName);

	// create wndclass 
	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = (LPCWSTR)GetWindowClassName();

	Debug::PrintAssert(RegisterClass(&wc),
		"Window class could not be registered.", __FILE__, __LINE__);

	// create handle
	
	hWnd = CreateWindowExW(
		0,                              // Optional window styles.
		GetWindowClassName(),			// Window class
		GetWindowClassName(),			// Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);
	
	SetLayeredWindowAttributes(hWnd, NULL, 255, LWA_ALPHA);

	Debug::PrintAssert(hWnd != NULL, "Window handle could not be created.",
		__FILE__, __LINE__);

	ShowWindow(hWnd, SW_NORMAL);
	
	Debug::PrintAssert(UpdateWindow(hWnd) != FALSE,
		"Failed to update the window.", __FILE__, __LINE__);
}

EditorWindow::~EditorWindow()
{

}

int EditorWindow::HandleMessages()
{
	MSG msg = { };

	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		if (msg.message == WM_QUIT)
		{
			break;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hwnd, &ps);

				// All painting occurs here, between BeginPaint and EndPaint.

				FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

				EndPaint(hwnd, &ps);
			}
			return 0;

		case WM_CLOSE:
			DestroyWindow(hwnd);
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

#pragma region getter / setter

HWND EditorWindow::GetWindowHandle()
{
	Debug::PrintAssert(sizeof(hWnd) < sizeof(byte),
		"Window handle not defined correctly!", __FILE__, __LINE__);
	return hWnd;
}

WNDCLASS EditorWindow::GetWindowClass()
{
	Debug::PrintAssert(sizeof(wc) < sizeof(byte), 
		"Window class not defined correctly!", __FILE__, __LINE__);

	return wc;
}

wchar_t* EditorWindow::GetWindowClassName()
{
	return CLASS_NAME;
}

void EditorWindow::SetWindowHandle(HWND& handle)
{
	Debug::PrintAssert(sizeof(handle) < sizeof(byte),
		"Window handle is not valid!", __FILE__, __LINE__);

	if (sizeof(handle) > sizeof(byte))
		hWnd = handle;
}

void EditorWindow::SetWindowClass(WNDCLASS& wndClass)
{
	Debug::PrintAssert(sizeof(wndClass) < sizeof(byte),
		"Window class is not valid!", __FILE__, __LINE__);

	if (sizeof(wndClass) > sizeof(byte))
		wc = wndClass;
}

void EditorWindow::SetWindowClassName(wchar_t* wndClassName)
{
	Debug::PrintAssert(wndClassName != L"", 
		"Window class name may not be empty!", __FILE__, __LINE__);

	if(wndClassName != L"")
		CLASS_NAME = wndClassName;
}

#pragma endregion



