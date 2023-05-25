#pragma once
#include <CCEngine.h>
#include <string>
#include <optional>
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

	bool OpenWindow(HINSTANCE hInstance, CCE::String winName = "Creative Chaos Engine - v0.1");
	void InitializeGUI();
	void UpdateEditorWindow(int& _returnVal);
	void PreGUIUpdate();
	void UpdateGUI();
	void PostGUIUpdate();
	int CloseEditorWindow();

	int GetEditorWindowWidth() const;
	int GetEditorWindowHeight() const;

	HWND GetEditorWindowHandle() const;
	WNDCLASS GetEditorWindowClass() const;
	CCE::Graphics::RenderPipeline* GetRenderPipeline();
	CCE::String GetEditorWindowName() const;
	void SetEditorWindowName(CCE::String name);

	bool windowRunning = false;
	bool demoWindowShowing = false;

private:
	bool imguiEnabled = true;
	CCE::String windowName = "";

private:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static CCE::InputManager* p_inputManager;
	WNDCLASS wndClass = {}; // TODO: Make this a reference to a style class
	HWND hWnd = {};
	CCE::Graphics::RenderPipeline renderPipeline;
};

