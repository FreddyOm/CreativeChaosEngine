#include "CCE_Editor.h"
#include "../../core/debug/CCException.h"
#include <optional>


#pragma region init / deinit

/// <summary>
/// Creates and registers the main windows class
/// </summary>
/// <returns></returns>
CCE_Editor::AppWindow::AppWindow() noexcept
    : hInst(GetModuleHandle(nullptr))
{
    // create wndclass
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = HandleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hIcon = nullptr;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = GetWindowClassName();
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassEx(&wc);
}

/// <summary>
/// Closes the Application Window
/// </summary>
CCE_Editor::AppWindow::~AppWindow()
{
    UnregisterClass(wndClassName, GetInstance());
}

/// <summary>
/// Constructor of the editor. Sets parameters and creates the main window
/// </summary>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="name"></param>
CCE_Editor::CCE_Editor(int width, int height, const char* name)
    : window_width(width), window_height(height), windowName(name)
{
    editor_running = true;
    appWnd = new AppWindow();

    // initial window

    CreateNewWindow();
}

/// <summary>
/// Closes the editor
/// </summary>
CCE_Editor::~CCE_Editor()
{
    appWnd->~AppWindow();
}

/// <summary>
/// Closes the editor
/// </summary>
void CCE_Editor::ExitEditor() noexcept
{
    editor_running = false;
    CCE_Editor::~CCE_Editor();
}

#pragma endregion init / deinit

#pragma region editor loop

int CCE_Editor::OpenEditor() noexcept
{
    while (true)
    {
        if (const auto ecode = ProcessMessages())
        {
            return *ecode;
        }
        UpdateEditor();
    }
    return 0;
}

std::optional<int> CCE_Editor::ProcessMessages() noexcept
{
    MSG msg;

    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return (int)msg.wParam;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return {};
}

bool CCE_Editor::UpdateEditor() const
{
    // Update

    return true;
}

#pragma endregion editor loop

#pragma region msg handling

/// <summary>
/// Handles the setup for the windows message loop
/// </summary>
/// <param name="hWnd">The windows handle</param>
/// <param name="msg">The message return value</param>
/// <param name="wParam">The wParam</param>
/// <param name="lParam">The lParam</param>
/// <returns>LRESULT with the specific information for the window loop</returns>
LRESULT WINAPI CCE_Editor::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    if (msg == WM_NCCREATE)
    {
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        CCE_Editor* const pWnd = static_cast<CCE_Editor*>(pCreate->lpCreateParams);

        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&CCE_Editor::HandleMsgThunk));

        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

/// <summary>
/// Handles the messages for the window and returns the specific testet input information
/// </summary>
/// <param name="hWnd">The windows handle</param>
/// <param name="msg">The message return value</param>
/// <param name="wParam">The wParam</param>
/// <param name="lParam">The lParam</param>
/// <returns>LRESULT with the specific information for the window loop</returns>
LRESULT WINAPI CCE_Editor::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    CCE_Editor* const pWnd = reinterpret_cast<CCE_Editor*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

/// <summary>
/// Handles specific message information like quit msgs and input
/// </summary>
/// <param name="hWnd">The windows handle</param>
/// <param name="msg">The message return value</param>
/// <param name="wParam">The wParam</param>
/// <param name="lParam">The lParam</param>
/// <returns>LRESULT with the specific information for the window loop</returns>
LRESULT CCE_Editor::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    case WM_KILLFOCUS:
        break;
    }

    //TODO: Handle input stuff

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

#pragma endregion msg handling

/// <summary>
/// The window class
/// </summary>
CCE_Editor::AppWindow CCE_Editor::AppWindow::wndClass;

/// <summary>
/// A condition check for whether or not the editor is still running
/// </summary>
/// <returns>True if editor is still running</returns>
bool CCE_Editor::EditorRunning() noexcept
{
    return editor_running;
}

/// <summary>
/// Creates a new Application Window and sets all relevant parameters
/// </summary>
void CCE_Editor::CreateNewWindow()
{
    //TODO: Create own wndMsgProc for children (PostQuitMsg closes parent too)
    
    HWND parentWindow = nullptr;
    RECT wr;

    if (!isInitalized)
    {
        isInitalized = true;

        // create the rect
        wr.left = 100;
        wr.right = window_width + wr.left;
        wr.top = 100;
        wr.bottom = window_height + wr.top;
        AdjustWindowRect(&wr, WS_CAPTION | WS_MAXIMIZE | WS_MAXIMIZEBOX | WS_SYSMENU | WS_SIZEBOX, FALSE);
    }
    else 
    {
        parentWindow = hWnd;

        // create the rect
        wr.left = 100;
        wr.right = window_width/2 + wr.left;
        wr.top = 100;
        wr.bottom = window_height/2 + wr.top;
        AdjustWindowRect(&wr, WS_CAPTION | WS_CHILD | WS_MINIMIZEBOX | WS_SYSMENU | WS_SIZEBOX, FALSE);
    }


    hWnd = CreateWindow(
        AppWindow::GetWindowClassName(), windowName,
        WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_SIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
        parentWindow, nullptr, AppWindow::GetInstance(), this
    );

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
}

/// <summary>
/// Sets the windows title displayed on the window frame
/// </summary>
/// <param name="title">The new title as a string</param>
void CCE_Editor::SetWindowTitle(const std::string& title)
{
    SetWindowTextA(hWnd, title.c_str());
}

#pragma region getter

/// <summary>
/// Fetches the windows class name
/// </summary>
/// <returns>Name of the main window class</returns>
const char* CCE_Editor::AppWindow::GetWindowClassName() noexcept
{
    return wndClassName;
}

/// <summary>
/// Returns the instance of the applications window
/// </summary>
/// <returns>HInstance of the window</returns>
HINSTANCE CCE_Editor::AppWindow::GetInstance() noexcept
{
    return wndClass.hInst;
}

#pragma endregion getter
