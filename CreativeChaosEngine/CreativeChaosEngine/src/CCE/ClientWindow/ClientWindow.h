#pragma once
#include <string>
#include <optional>
#include <vector>
#include "../Core.h"
#include "../Manager/InputManager.h"
#include "../Manager/JobManager.h"
#include "../Graphics/RenderPipeline.h"

// TODO: Refactor static manager pointers to work in a defined way

struct CCE_API ClientWindow
{ 
	ClientWindow() = default;

	~ClientWindow()
	{
        windowRunning = false;
		Instance = nullptr;
	}

	bool OpenWindow(HINSTANCE hInstance, CCE::String winName = "Creative Chaos Engine - v0.1");
	void UpdateClientWindow(int& _returnVal);
	int CloseClientWindow();

	int GetClientWindowWidth() const;
	int GetClientWindowHeight() const;

	HWND GetClientWindowHandle() const;
	WNDCLASS GetClientWindowClass() const;
	CCE::Graphics::RenderPipeline* GetRenderPipeline();
	CCE::String GetClientWindowName() const;
	void SetClientWindowName(CCE::String name);

	bool windowRunning = false;
	bool demoWindowShowing = false;
	bool fullScreen = false;

private:
	static ClientWindow* Instance;
	CCE::String windowName = "";
private:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	WNDCLASS wndClass = {}; // TODO: Make this a reference to a style class
	HWND hWnd = {};
	CCE::Graphics::RenderPipeline renderPipeline;
};
