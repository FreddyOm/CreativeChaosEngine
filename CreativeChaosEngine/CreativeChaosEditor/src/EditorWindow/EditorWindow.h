#pragma once
#include <CCEngine.h>
#include <string>
#include "../CreativeChaosEditor.h"

struct EditorWindow
{ 
	EditorWindow() = default;
	EditorWindow(CCE::InputManager* pIM)
	{
		p_inputManager = pIM;
	}
	~EditorWindow()
	{
        windowRunning = false;
	}

	int OpenWindow(HINSTANCE hInstance, CCE::String winName = "Creative Chaos Engine - v0.1");
	int UpdateWindow() const;
	int CloseEditorWindow();

	HWND GetEditorWindowHandle() const;
	WNDCLASS GetEditorWindowClass() const;
	CCE::String GetEditorWindowName() const;
	void SetEditorWindowName(CCE::String name);

	bool windowRunning = false;

private:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static CCE::InputManager* p_inputManager;
	CCE::String windowName = "";
	WNDCLASS wndClass = {}; // TODO: Make this a reference to a style class
	HWND hWnd = {};
};

