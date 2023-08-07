#pragma once
#include <string>
#include <optional>
#include <vector>
#include "../Core.h"
#include "../Manager/InputManager.h"
#include "../Manager/JobManager.h"
#include "../Graphics/RenderPipeline.h"
#include "../Manager/ProfilingManager.h"

// TODO: Refactor static manager pointers to work in a defined way
namespace CCE
{
	struct CCE_API ClientWindow
	{
	public:
		ClientWindow()
		{
			REGISTER_LEAK_DETECT;
			Instance = this;
		}

		~ClientWindow()
		{
			windowRunning = false;
			Instance = nullptr;
			UNREGISTER_LEAK_DETECT;
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
		bool minimized = false;

		static ClientWindow* Instance;
	private:
		CCE::String windowName = "";
	private:
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		WNDCLASS wndClass = {}; // TODO: Make this a reference to a style class
		HWND hWnd = {};
		CCE::Graphics::RenderPipeline renderPipeline;
	};
}
