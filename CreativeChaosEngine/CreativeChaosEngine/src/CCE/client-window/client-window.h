#pragma once
#include <string>
#include <optional>
#include <vector>
#include "../core.h"
#include "../input/input.h"
#include "../graphics/rendering.h"
#include "../manager/profilingManager.h"
#include "../multithreading/job-system.h"

// @TODO: Refactor static manager pointers to work in a defined way
namespace CCE
{
	class CCE_API ClientWindow
	{
	public:
		ClientWindow()
			: windowName("Creative Chaos Engine - v0.1")
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

		bool OpenWindow(HINSTANCE hInstance, String winName = "Creative Chaos Engine - v0.1");
		Jobs::JobReturnType UpdateClientWindow(uintptr_t _returnVal);
		int CloseClientWindow();

		int GetClientWindowWidth() const;
		int GetClientWindowHeight() const;

		HWND GetClientWindowHandle() const;
		WNDCLASS GetClientWindowClass() const;
		String GetClientWindowName() const;
		void SetClientWindowName(String name);

		bool windowRunning = false;
		bool demoWindowShowing = false;
		bool minimized = false;

		static ClientWindow* Instance;
	private:
		String windowName = "";
	private:
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		WNDCLASS wndClass = {}; // @TODO: Make this a reference to a style class
		HWND hWnd = {};
	};
}
