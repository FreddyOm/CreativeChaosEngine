#include "EditorWindow.h"
#include "CCE/Manager/InputManager.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#include "CCE/Manager/JobManager.h"

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

	DASSERT(GetEditorWindowHandle() != NULL, "Window handle is invalid!");
	
	SetWindowTextA(GetEditorWindowHandle(), windowName.Value());
	ShowWindow(GetEditorWindowHandle(), SW_NORMAL); // Returns nonzero if previously visible
	windowRunning = true;

	// Init d3d11 for this editor window
	LOG_REND("Initializing Direct3D...");
	renderPipeline.InitializeD3D11(hWnd, GetEditorWindowWidth(), GetEditorWindowHeight());

	// Init ImGui
	LOG_REND("Initializing GUI...");
	InitializeGUI();
	
	return windowRunning;
}

/// <summary>
/// Initializing ImGui with D3D11 and ImGui
/// </summary>
void EditorWindow::InitializeGUI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	io.Fonts->AddFontFromFileTTF("resources/fonts/Lexend-Light.ttf", 14);

	ImGui::StyleColorsCCE();

	// Hook the editors input calls to the engines input
	CCE::InputManager::Instance->inputCallback = &ImGui_ImplWin32_WndProcHandler;

	DASSERT(ImGui_ImplDX11_Init(renderPipeline.GetDevicePtr(), renderPipeline.GetDeviceContextPtr()),
		"Failed initializing GUI with D3D11.");
	DASSERT(ImGui_ImplWin32_Init(GetEditorWindowHandle()),
		"Failed initializing GUI with Editor Window.");
}

/// <summary>
/// The window's message pump.
/// </summary>
/// <returns>The return code whenever the window is closed.</returns>
void EditorWindow::UpdateEditorWindow(int& _returnVal)
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
/// Do GUI setup here.
/// </summary>
void EditorWindow::UpdateGUI()
{
	if (ImGui::Begin("Frametime Debugging"))
	{
		ImGui::Text("FPS: %d", (short)(1000.0 / CCE::Time::deltaTime));
		ImGui::Text("Frametime (ms): %f", CCE::Time::deltaTime);
		ImGui::Text("Avg. Frametime (ms): %f", CCE::Time::GetAverageFrameTime());
	
		ImGui::Spacing();
		ImGui::Checkbox("Fullscreen", & fullScreen);
	}

	ImGui::End();
	//ImGui::ShowDemoWindow(&demoWindowShowing);
}

/// <summary>
/// Render the GUI.
/// </summary>
void EditorWindow::PostGUIUpdate()
{
	if (imguiEnabled)
	{
		ImGui::Render();
		ImDrawData* drawData = ImGui::GetDrawData();
		ImGui_ImplDX11_RenderDrawData(drawData);
	}
}

/// <summary>
/// Set new GUI frame.
/// </summary>
void EditorWindow::PreGUIUpdate()
{
	if (imguiEnabled)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
}

/// <summary>
/// Closes the editor window.
/// </summary>
/// <returns>Return code for more detailed info.</returns>
int EditorWindow::CloseEditorWindow()
{
	windowRunning = false;
	ImGui_ImplDX11_Shutdown();
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
	// TODO: Handle input in one place & make explicit code platform independent
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
	case WM_PAINT:
	{

	}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

/// <summary>
/// Pointer to input manager.
/// </summary>
CCE::InputManager* EditorWindow::p_inputManager = nullptr;

/// <summary>
/// Pointer to render pipeline
/// </summary>
CCE::Graphics::RenderPipeline* EditorWindow::p_renderPipeline = nullptr;
