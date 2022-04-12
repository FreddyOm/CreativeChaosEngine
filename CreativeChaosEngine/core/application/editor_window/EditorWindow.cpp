#include "EditorWindow.h"
#include "../../core/debug/Debug.h"

EditorWindow::EditorWindow(unsigned int width, unsigned int height, std::wstring winName, unsigned short windowID, WNDPROC proc) : m_width(width), m_height(height)
{
    m_windowClass.hInstance = m_hinst;
    m_windowClass.lpszClassName = m_windowClassName;
    m_windowClass.hCursor = LoadCursor(nullptr, IDC_HAND);
    m_windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
    m_windowClass.lpfnWndProc = proc;
    RegisterClass(&m_windowClass);

    // window dimensions
    m_windowRect.left = 100;
    m_windowRect.right = width + m_windowRect.left;
    m_windowRect.top = 100;
    m_windowRect.bottom = height + m_windowRect.top;

    Debug::PrintAssert(AdjustWindowRect(&m_windowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE),
        "Adjusting the window rect was unsuccessful!");

    m_hWnd = CreateWindowEx(
        CS_OWNDC,               // no extended styles           
        m_windowClassName,      // class name                   
        winName.c_str(),        // window name                  
        WS_OVERLAPPEDWINDOW |   // overlapped window    
        WS_VISIBLE,             // visible idk
        CW_USEDEFAULT,          // default horizontal position  
        CW_USEDEFAULT,          // default vertical position    
        m_windowRect.right - m_windowRect.left,
        m_windowRect.bottom - m_windowRect.top,
        (HWND)NULL,             // no parent or owner window    
        (HMENU)NULL,            // class menu used              
        m_hinst,                // instance handle              
        NULL);                  // no window creation data 

    m_windowID = windowID;

    ShowWindow(m_hWnd, SW_SHOWDEFAULT);
}

EditorWindow::~EditorWindow()
{
    UnregisterClass(m_windowClassName, m_hinst);
    DestroyWindow(m_hWnd);

    m_editorWindowName = NULL;
    m_windowClassName = NULL;
    m_hinst = NULL;
    m_hWnd = NULL;
    m_windowClass = {};

    delete m_hinst;
    delete m_hWnd;
    delete m_editorWindowName;
    delete m_windowClassName;
}

/// <summary>
/// Set the editor windows title.
/// </summary>
/// <param name="name">The new title</param>
void EditorWindow::SetEditorWindowName(std::wstring name)
{
    m_editorWindowName = name.c_str();

    //update window name

    SetWindowTextW(m_hWnd, m_editorWindowName);
}

/// <summary>
/// Gets the current Windows title.
/// </summary>
/// <returns>A wide string pointer to the current title</returns>
LPCWSTR* EditorWindow::GetEditorWindowName()
{
    return &m_editorWindowName;
}

/// <summary>
/// Sets the windows width
/// </summary>
/// <param name="width">The new width</param>
void EditorWindow::SetEditorWindowWidth(unsigned int width)
{
    m_width = width;

    // update window
    m_windowRect.right = m_width + m_windowRect.left;

    Debug::PrintAssert(AdjustWindowRect(&m_windowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE),
        "Adjusting the window rect was unsuccessful!");

}

/// <summary>
/// Sets the windows height
/// </summary>
/// <param name="height">The new height</param>
void EditorWindow::SetEditorWindowHeight(unsigned int height)
{
    m_height = height;

    // update window
    m_windowRect.bottom = m_height + m_windowRect.top;

    Debug::PrintAssert(AdjustWindowRect(&m_windowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE),
        "Adjusting the window rect was unsuccessful!");

}

/// <summary>
/// Retruns the width of the current window
/// </summary>
/// <returns>Width</returns>
unsigned int* EditorWindow::GetEditorWindowWidth()
{
    return &m_width;
}

/// <summary>
/// Retruns the height of the current window
/// </summary>
/// <returns>Height</returns>
unsigned int* EditorWindow::GetEditorWindowHeight()
{
    return &m_height;
}

/// <summary>
/// Updates the window
/// </summary>
/// <returns>False if an error occurs; True otherwise</returns>
bool EditorWindow::UpdateWindow()
{
    if (GetMessage(&msg, nullptr, 0, 0))
    {
        // Msg loop
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        return true;
    }
    else
    {
        return false;
    }
}