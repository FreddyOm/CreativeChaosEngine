#include "ClientWindow.h"
#include "../Manager/Application.h"
#include <functional>

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
bool CCE::ClientWindow::OpenWindow(HINSTANCE hInstance, CCE::String winName)
{
	// Set window name	
	windowName = winName;
	// Create and register win class
	wndClass.lpfnWndProc = WindowProc;
	wndClass.hInstance = hInstance;
	wndClass.lpszClassName = L"DefaultEditorWindow";

	RegisterClass(&wndClass);

	// Create and initialize hWnd
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

	DASSERT(GetClientWindowHandle() != NULL, "Window handle is invalid!");
	
	SetWindowTextA(GetClientWindowHandle(), windowName.Value());
	ShowWindow(GetClientWindowHandle(), SW_NORMAL); // Returns nonzero if previously visible
	windowRunning = true;

	// Init d3d11 for this editor window
	LOG_REND("Initializing Direct3D...");
	renderPipeline.InitializeD3D11(hWnd, GetClientWindowWidth(), GetClientWindowHeight());

	return windowRunning;
}

/// <summary>
/// The window's message pump.
/// </summary>
void CCE::ClientWindow::UpdateClientWindow(int& _returnVal)
{
	_returnVal = 0;
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == (int)WM_QUIT)
		{
			_returnVal = (int)msg.message;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

/// <summary>
/// Closes the editor window.
/// </summary>
/// <returns>Return code for more detailed info.</returns>
int CCE::ClientWindow::CloseClientWindow()
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
int CCE::ClientWindow::GetClientWindowWidth() const
{
	RECT rect = {};
	GetWindowRect(hWnd,&rect);

	return rect.right - rect.left;
}

/// <summary>
/// Get the windows height.
/// </summary>
/// <returns></returns>
int CCE::ClientWindow::GetClientWindowHeight() const
{
	RECT rect = {};
	GetWindowRect(hWnd, &rect);

	return rect.bottom - rect.top;
}

/// <summary>
/// Get the current editor window's handle.
/// </summary>
/// <returns>Window handle.</returns>
HWND CCE::ClientWindow::GetClientWindowHandle() const
{
	return hWnd;
}

/// <summary>
/// Get the current editor window's class.
/// </summary>
/// <returns>Window class.</returns>
WNDCLASS CCE::ClientWindow::GetClientWindowClass() const
{
	return wndClass;
}

/// <summary>
/// Get the current editor window's name.
/// </summary>
/// <returns>Window name.</returns>
CCE::String CCE::ClientWindow::GetClientWindowName() const
{
	return windowName;
}

/// <summary>
/// Set the current editor window's name.
/// </summary>
/// <param name="">Window name.</param>
void CCE::ClientWindow::SetClientWindowName(CCE::String name)
{
	windowName = name;
	SetWindowTextA(GetClientWindowHandle(), windowName.Value());
}

CCE::Graphics::RenderPipeline* CCE::ClientWindow::GetRenderPipeline()
{
	return &renderPipeline;
}

using namespace CCE;

/// <summary>
/// Window Procedure.
/// </summary>
/// <param name="hwnd"></param>
/// <param name="uMsg"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns>Result code.</returns>
LRESULT CALLBACK ClientWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// TODO: Handle input in one place & make explicit code platform independent
	// TODO: Create possibility to set values to 0 again
	InputManager::Instance->HandleWinInput(hwnd, uMsg, wParam, lParam);

	switch (uMsg)
	{
	case WM_SIZE:
	{
		if (wParam == SIZE_MINIMIZED)
		{
			ClientWindow::Instance->minimized = true;
		}
		else if (ClientWindow::Instance->minimized)
		{
			ClientWindow::Instance->minimized = false;
		}

		int width = LOWORD(lParam);  // Macro to get the low-order word.
		int height = HIWORD(lParam); // Macro to get the high-order word.

		// Update D3D11 and GUI
		if (CCE::Graphics::RenderPipeline::Instance->GetDeviceContextPtr() == NULL) { break; }
		CCE::Graphics::RenderPipeline::Instance->OnResize(hwnd, static_cast<UINT>(wParam), width, height);
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
/// Pointer to this instance.
/// </summary>
CCE::ClientWindow* CCE::ClientWindow::Instance = nullptr;
