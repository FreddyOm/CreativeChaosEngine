#include "EditorWindow.h"
#include <string.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/// <summary>
/// Opens an editor window with a default window name.
/// </summary>
/// <param name="hInstance"></param>
/// <param name="winName"></param>
/// <returns>A return code that indicates whether or not the window opened correctly.</returns>
int EditorWindow::OpenWindow(HINSTANCE hInstance, CCE::String winName)
{
	// Set window name	
	windowName = winName;

	// create and register win class
	wndClass.lpfnWndProc = WindowProc;
	wndClass.hInstance = hInstance;
	wndClass.lpszClassName = L"DefaultEditorWindow";

	RegisterClass(&wndClass);

	// create and initialize hWnd
	hWnd = CreateWindowEx(
		0,                              // Optional window styles.
		wndClass.lpszClassName,			// Window class
		L"This should not be visible - go to EditorWindow -> CreateWindowEx",	// Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	DASSERT(hWnd != NULL, "Window handle is invalid!");
	
	SetWindowTextA(hWnd, windowName.Value());
	ShowWindow(hWnd, SW_NORMAL); // Returns nonzero if previously visible
	windowRunning = true;

	return 0;
}

/// <summary>
/// Initializes the window's message pump.
/// </summary>
/// <returns>The return code whenever the window is closed.</returns>
int EditorWindow::UpdateWindow()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}

/// <summary>
/// Closes 
/// </summary>
/// <returns></returns>
int EditorWindow::CloseEditorWindow()
{
	DASSERT(DestroyWindow(hWnd) != 0, "Failed destroying the editor window!");
	LOG(GetLastErrorAsString().c_str());
	LOG("Window was implicitly closed.");
	windowRunning = false;
	PostQuitMessage(0);
	return WM_CLOSE;
}

HWND EditorWindow::GetEditorWindowHandle()
{
	return hWnd;
}

WNDCLASS EditorWindow::GetEditorWindowClass()
{
	return wndClass;
}

CCE::String EditorWindow::GetEditorWindowName()
{
	return windowName;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
	{
		int width = LOWORD(lParam);  // Macro to get the low-order word.
		int height = HIWORD(lParam); // Macro to get the high-order word.

		// Respond to the message:
		//OnSize(hwnd, (UINT)wParam, width, height);
		break;
	}
	case WM_CLOSE:
	{
		LOG("User closed window manually!");
		DestroyWindow(hwnd);
		PostQuitMessage(0);
		break;
	}
	// TODO: Handle Input and write it into an input class
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
