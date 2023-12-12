#pragma once
#include "BaseManager.h"
#include "../Input/InputDevice.h"
#include "../Input/IInputHandler.h"
#include "../Input/Controller.h"
#include "../Input/Keyboard.h"
#include "../Input/Mouse.h"
#include "../String/String.h"
#include <vector>

#include <Xinput.h>
#pragma comment(lib, "XInput.lib")
#pragma comment(lib, "D:/Repositories/CreativeChaosEngine/CreativeChaosEngine/CreativeChaosEngine/resources/sdk/ds5w_x64.lib")
//#pragma comment(lib, "ds5w_x64.lib")

#include "../include/ds5w/ds5w.h"

namespace CCE
{
	struct CCE_API InputManager : public BaseManager
	{
		typedef LRESULT (*GuiInputCallback)(HWND, UINT, WPARAM, LPARAM);
		friend class Application;
		friend class ClientWindow;
	public:
		InputManager() = default;
		~InputManager()
		{
			handlerList.~vector();
		}

		void StartUp() override;
		void ShutDown() override;

		
		void RegisterInputCallback(Input::IInputHandler& handler)
		{
			handlerList.push_back(&handler);
		}

		static InputManager* Instance;
		GuiInputCallback inputCallback = NULL;

	private:
		void HandleWinInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void HandleXInput();
		void HandleDirectInput();
		void InitializeDualSense();
		void HandleDualSenseInput();
		
		void FinalizeWinInput();
		void ResetInputValues();

	public:
		alignas (64)	Input::Mouse mouse = {};
		alignas (256)	Input::Keyboard keyboard = {};
		alignas (128)	Input::Controller controller[4] = {};
	
	private:
		Input::Controller* _currentController = nullptr;			// 8 bytes
		unsigned char connectedDeviceCount = 0;						// 1 byte
		unsigned char lastConnectedDeviceCount = 0;					// 1 byte
		std::vector<bool> activeController = { false, false, false, false };
		std::vector<bool> lastActiveController = { false, false, false, false };
		// xinput
		XINPUT_STATE state = {};
		
		// dual sense
		DS5W::DeviceEnumInfo infos[XUSER_MAX_COUNT];
		DS5W::DeviceContext con[XUSER_MAX_COUNT];
		DS5W::DS5InputState inState[XUSER_MAX_COUNT];
		DS5W::DS5OutputState outState[XUSER_MAX_COUNT];

		// InputHandler
		std::vector<Input::IInputHandler*> handlerList;

	private:

		void UpdateXInputControllerCount();
		void GetXInput(const DWORD controller_Index);
	};	
}
