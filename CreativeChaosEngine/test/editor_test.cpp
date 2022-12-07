#include "editor_test.h"


#ifdef _DEBUG
#include <iostream>
#include "../core/application/editor/EditorWindow.h"
#include "../core/debug/Debug.h"
using namespace CCE_Debug;

/// <summary>
/// Editor test constructor.
/// </summary>
EditorTest::EditorTest()
{
	editor = new CCE_Editor();
	window = new EditorWindow();
}

/// <summary>
/// Editor test destructor.
/// </summary>
EditorTest::~EditorTest()
{
	free(editor);
	free(window);
}

void EditorTest::RunAllTests()
{
	if (TestEditorRunning())
	{
		PrintTestSuccessInfo(L"Test Editor Running");
	}
	else
	{
		PrintTestFailInfo(L"Test Editor Running", __WFILE__, __LINE__);
	}

	if (TestEditorExit())
	{
		PrintTestSuccessInfo(L"Test Editor Exit");
	}
	else
	{
		PrintTestFailInfo(L"Test Editor Exit", __WFILE__, __LINE__);
	}

	if (TestWindowHandle())
	{
		PrintTestSuccessInfo(L"Test Window Handle");
	}
	else
	{
		PrintTestFailInfo(L"Test Window Handle", __WFILE__, __LINE__);
	}

	if (TestWindowClass())
	{
		PrintTestSuccessInfo(L"Test Window Class");
	}
	else
	{
		PrintTestFailInfo(L"Test Window Class", __WFILE__, __LINE__);
	}

	if (TestWindowClassName())
	{
		PrintTestSuccessInfo(L"Test Window Class Name");
	}
	else
	{
		PrintTestFailInfo(L"Test Window Class Name", __WFILE__, __LINE__);
	}

	if (TestWindowSize())
	{
		PrintTestSuccessInfo(L"Test Window Size");
	}
	else
	{
		PrintTestFailInfo(L"Test Window Size", __WFILE__, __LINE__);
	}

}

/// <summary>
/// Tests the editor running function of the cce editor class.
/// </summary>
/// <returns>False if unexpected results are registered; True if everything works fine.</returns>
bool EditorTest::TestEditorRunning()
{
	bool firstTest = false;
	editor->editorRunning = true;
	firstTest = editor->editorRunning == editor->EditorRunning();

	bool secondTest = false;
	editor->editorRunning = false;
	secondTest = editor->editorRunning == editor->EditorRunning();

	return firstTest && secondTest;
}

/// <summary>
/// Tests the editor exit function of the cce editor class.
/// </summary>
/// <returns>False if unexpected results are registered; True if everything works fine.</returns>
bool EditorTest::TestEditorExit()
{
	bool firstTest = false;
	editor->editorRunning = true;
	editor->editorWindow = new EditorWindow(HINSTANCE{ 0 }, L"Test");
	editor->ExitEditor();
	firstTest = editor->editorRunning == false && editor->editorWindow == nullptr;

	bool secondTest = false;
	editor->editorRunning = true;
	editor->editorWindow = new EditorWindow(HINSTANCE{ 0 }, L"Test");
	editor->~CCE_Editor();
	secondTest = editor->EditorRunning() == false && editor->editorWindow == nullptr;

	return firstTest && secondTest;
}

/// <summary>
/// Tests the window handle getter/setter function of the editor window class.
/// </summary>
/// <returns>False if unexpected results are registered; True if everything works fine.</returns>
bool EditorTest::TestWindowHandle()
{
	bool firstTest = false;
	HWND wnd = CreateWindowExW(
		0,                              // Optional window styles.
		L"TestClassName",				// Window class
		L"TestWindowTitle",				// Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window
		NULL,       // Menu
		HINSTANCE{},  // Instance handle
		NULL        // Additional application data
	);
	window->SetWindowHandle(wnd);
	firstTest = window->hWnd != NULL
		&& window->GetWindowHandle() == window->hWnd
		&& window->GetWindowHandle() != NULL;


	bool secondTest = false;
	HWND wnd2 = { 0 };
	window->SetWindowHandle(wnd2);

	secondTest = window->GetWindowHandle() != NULL
		&& window->GetWindowHandle() != wnd2
		&& window->hWnd != wnd2
		&& window->hWnd == wnd;

	return firstTest && secondTest;
}

/// <summary>
/// Tests the window class getter/setter function of the editor window class.
/// </summary>
/// <returns>False if unexpected results are registered; True if everything works fine.</returns>
bool EditorTest::TestWindowClass()
{
	bool firstTest = false;

	auto hInst = (HINSTANCE)malloc(sizeof(HINSTANCE));

	WNDCLASS wc2 = {};

	wc2.lpfnWndProc = (WNDPROC)malloc(sizeof(WNDPROC));
	wc2.hInstance = hInst;
	wc2.lpszClassName = L"TestClassName";

	window->wc = wc2;

	firstTest = RegisterClass(&window->GetWindowClass());

	bool secondTest = false;
	WNDCLASS wc3 = {};
	window->SetWindowClass(wc3);
	window->SetWindowClass(wc2);

	secondTest = UnregisterClass(L"TestClassName", hInst);


	free(wc2.lpfnWndProc);
	free(hInst);

	return firstTest && secondTest;
}

/// <summary>
/// Tests the window class name getter/setter function of the editor window class.
/// </summary>
/// <returns>False if unexpected results are registered; True if everything works fine.</returns>
bool EditorTest::TestWindowClassName()
{
	bool firstTest = false;
	window->CLASS_NAME = L"MyTestClassName";

	firstTest = window->CLASS_NAME == window->GetWindowClassName()
		&& window->GetWindowClassName() == L"MyTestClassName";

	bool secondTest = false;

	window->SetWindowClassName(L"MySecondTestClassName");

	secondTest = window->CLASS_NAME == window->GetWindowClassName()
		&& window->CLASS_NAME == L"MySecondTestClassName";

	return firstTest && secondTest;
}

/// <summary>
/// Tests the window size getter/setter function of the editor window class.
/// </summary>
/// <returns>False if unexpected results are registered; True if everything works fine.</returns>
bool EditorTest::TestWindowSize()
{
	bool firstTest = false;
	window->win_height = 300;
	window->win_width = 500;

	firstTest = window->win_height == window->GetWindowHeight()
		&& window->win_width == window->GetWindowWidth()
		&& window->win_height == 300 && window->win_width == 500;

	bool secondTest = false;

	window->SetWindowHeight(830);
	window->SetWindowWidth(1490);

	secondTest = window->win_height == window->GetWindowHeight()
		&& window->win_width == window->GetWindowWidth()
		&& window->win_height == 830 && window->win_width == 1490;

	return firstTest && secondTest;
}
#endif // only test if DEBUG
