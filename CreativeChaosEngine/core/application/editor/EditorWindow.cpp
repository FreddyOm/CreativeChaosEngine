#include "EditorWindow.h"
#include <iostream>
#include "../../debug/Debug.h"

using namespace CCE_Debug;

/// <summary>
/// The window message procedure.
/// </summary>
/// <param name="hwnd">Handle to the window.</param>
/// <param name="uMsg">Message queried by the procedure.</param>
/// <param name="wParam">Parameter.</param>
/// <param name="lParam">Parameter.</param>
/// <returns>Status of the message procedure.</returns>
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#pragma region init / deinit

#if _DEBUG
/// <summary>
/// Standard constructor for testing purposes only.
/// </summary>
EditorWindow::EditorWindow()
{

}
#endif // unit testing


/// <summary>
/// The constructor of the editor window class.
/// </summary>
/// <param name="hInstance">The handle to the applications instance.</param>
/// <param name="windowName">The name of the window (and the window class).</param>
EditorWindow::EditorWindow(HINSTANCE hInstance, wchar_t* windowName)
{
	// set hInstance
	hInst = &hInstance;

	// set window class name
	SetWindowClassName(windowName);

	// create wndclass 
	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = (LPCWSTR)GetWindowClassName();

	ASSERT(RegisterClass(&wc), L"Window class could not be registered.");

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

	ASSERT(hWnd != NULL, L"Window handle could not be created.");

	ShowWindow(hWnd, SW_NORMAL);

	ASSERT(UpdateWindow(hWnd) != FALSE, L"Failed to update the window.");
}

/// <summary>
/// Default destructor of the editor window class.
/// </summary>
EditorWindow::~EditorWindow()
{
	UnregisterClass((LPCWSTR)GetWindowClassName(), *hInst);
}

#pragma endregion

#pragma region message handling

/// <summary>
/// Handles the messages queried by this window.
/// </summary>
/// <returns>Status about the application.</returns>
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



/// <summary>
/// Callback for the window's procedure.
/// </summary>
/// <param name="hwnd">Handle to the window.</param>
/// <param name="uMsg">Message queried by the procedure.</param>
/// <param name="wParam">Parameter.</param>
/// <param name="lParam">Parameter.</param>
/// <returns>Status of the message procedure.</returns>
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

#pragma endregion

#pragma region getter / setter

/// <summary>
/// Gets the handle of this window.
/// </summary>
/// <returns>The handle of this window.</returns>
HWND EditorWindow::GetWindowHandle()
{
	return hWnd;
}

/// <summary>
/// Gets the class of this window.
/// </summary>
/// <returns>The class of this window.</returns>
WNDCLASS EditorWindow::GetWindowClass()
{
	return wc;
}

/// <summary>
/// Gets the window class name
/// </summary>
/// <returns>The class name of this window</returns>
wchar_t* EditorWindow::GetWindowClassName()
{
	return CLASS_NAME;
}

unsigned int EditorWindow::GetWindowHeight()
{
	return win_height;
}

unsigned int EditorWindow::GetWindowWidth()
{
	return win_width;
}

/// <summary>
/// Sets the handle of this window.
/// </summary>
/// <param name="handle">The (new) handle of this window.</param>
void EditorWindow::SetWindowHandle(HWND& handle)
{
	if (sizeof(handle) > sizeof(byte))
		hWnd = handle;
}

/// <summary>
/// Sets the class of this window.
/// </summary>
/// <param name="wndClass">The (new) class of this window.</param>
void EditorWindow::SetWindowClass(WNDCLASS& wndClass)
{
	if (sizeof(wndClass) > sizeof(byte))
		wc = wndClass;
}

/// <summary>
/// Sets the class name of this window.
/// </summary>
/// <param name="wndClassName">The (new) class name of this window.</param>
void EditorWindow::SetWindowClassName(wchar_t* wndClassName)
{
	if (wndClassName != L"")
		CLASS_NAME = wndClassName;
}

void EditorWindow::SetWindowHeight(const unsigned int height)
{
	win_height = height;
}

void EditorWindow::SetWindowWidth(const unsigned int width)
{
	win_width = width;
}

#pragma endregion
