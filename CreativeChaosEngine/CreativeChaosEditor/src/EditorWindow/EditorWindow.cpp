#include "EditorWindow.h"
#include <string.h>

/// <summary>
/// Callback for window procedure.
/// </summary>
/// <param name="hwnd"></param>
/// <param name="uMsg"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns>Result code.</returns>
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/// <summary>
/// Opens an editor window with a default window name.
/// </summary>
/// <param name="hInstance"></param>
/// <param name="winName"></param>
/// <returns>A return code that indicates whether or not the window opened correctly.</returns>
bool EditorWindow::OpenWindow(HINSTANCE hInstance, CCE::String winName)
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

	DASSERT(GetEditorWindowHandle() != NULL, "Window handle is invalid!");
	
	SetWindowTextA(GetEditorWindowHandle(), windowName.Value());
	ShowWindow(GetEditorWindowHandle(), SW_NORMAL); // Returns nonzero if previously visible
	windowRunning = true;

	// init d3d11 for this editor window
	renderPipeline.InitializeD3D11(hWnd, GetEditorWindowWidth(), GetEditorWindowHeight());

	return windowRunning;
}

/// <summary>
/// Initializes the window's message pump.
/// </summary>
/// <returns>The return code whenever the window is closed.</returns>
std::optional<int> EditorWindow::UpdateEditorWindow() const
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == (int)WM_QUIT)
		{
			return (int)msg.message;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
}

/// <summary>
/// Closes the editor window.
/// </summary>
/// <returns>Return code for more detailed info.</returns>
int EditorWindow::CloseEditorWindow()
{
	windowRunning = false;
	DASSERT(DestroyWindow(hWnd) != 0, "Failed destroying the editor window!");
	LOG("Window was implicitly closed.");
	PostQuitMessage(0);
	return WM_CLOSE;
}

/// <summary>
/// Get the windows width.
/// </summary>
/// <returns></returns>
int EditorWindow::GetEditorWindowWidth() const
{
	RECT rect = {};
	GetWindowRect(hWnd,&rect);

	return rect.right - rect.left;
}

/// <summary>
/// Get the windows height.
/// </summary>
/// <returns></returns>
int EditorWindow::GetEditorWindowHeight() const
{
	RECT rect = {};
	GetWindowRect(hWnd, &rect);

	return rect.bottom - rect.top;
}

/// <summary>
/// Get the current editor window's handle.
/// </summary>
/// <returns>Window handle.</returns>
HWND EditorWindow::GetEditorWindowHandle() const
{
	return hWnd;
}

/// <summary>
/// Get the current editor window's class.
/// </summary>
/// <returns>Window class.</returns>
WNDCLASS EditorWindow::GetEditorWindowClass() const
{
	return wndClass;
}

/// <summary>
/// Get the render pipeline.
/// </summary>
/// <returns></returns>
CCE::Graphics::RenderPipeline* EditorWindow::GetRenderPipeline()
{
	return &renderPipeline;
}

/// <summary>
/// Get the current editor window's name.
/// </summary>
/// <returns>Window name.</returns>
CCE::String EditorWindow::GetEditorWindowName() const
{
	return windowName;
}

/// <summary>
/// Set the current editor window's name.
/// </summary>
/// <param name="">Window name.</param>
void EditorWindow::SetEditorWindowName(CCE::String name)
{
	windowName = name;
	SetWindowTextA(GetEditorWindowHandle(), windowName.Value());
}

/// <summary>
/// Window Procedure.
/// </summary>
/// <param name="hwnd"></param>
/// <param name="uMsg"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns>Result code.</returns>
LRESULT CALLBACK EditorWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// TODO: Handle Input and write it into an input class
	// TODO: Handle Input in one place & make explicit code platform independent
	// TODO: Create possibility to set values to 0 again
	p_inputManager->HandleWinInput(hwnd, uMsg, wParam, lParam);

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
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

/// <summary>
/// Pointer to input manager.
/// </summary>
CCE::InputManager* EditorWindow::p_inputManager = nullptr;
