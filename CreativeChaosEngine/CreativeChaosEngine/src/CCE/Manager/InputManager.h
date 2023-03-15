#pragma once
#include "BaseManager.h"
#include "../Input/InputDevice.h"
#include "../String/String.h"
#include <vector>

#include <Xinput.h>
#pragma comment(lib, "XInput.lib")

namespace CCE
{
	struct CCE_API InputManager : public BaseManager
	{
	public:
		InputManager() = default;
		~InputManager() = default;

		void StartUp() override;
		void ShutDown() override;

		static InputManager* Instance;

		void HandleWinInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void HandleXInput();
		void HandleDirectInput();

	private:
		
		// TODO: Keep left handed mouse layout in mind
		// TODO: Store input values per bit in DWORD (or similar)
		struct Mouse : private Input::InputDevice  // 64 bytes
		{
			int xPos = 0;						// 4 bytes
			int yPos = 0;						// 4 bytes
			int deltaX = 0;						// 4 bytes
			int deltaY = 0;						// 4 bytes

			int lastXPos = 0;					// 4 bytes
			int lastYPos = 0;					// 4 bytes
			float wheelDelta = 0;				// 4 bytes
			float lastWheelDelta = 0;			// 4 bytes

			ButtonState rightMouseButton = {};	// 4 bytes
			ButtonState leftMouseButton = {};	// 4 bytes
			ButtonState middleMouseButton = {};	// 4 bytes
			ButtonState extraButton1 = {};		// 4 bytes
			
			ButtonState extraButton2 = {};		// 4 bytes
			byte padding[12] = {};				// 12 bytes
			
		};

		struct Keyboard : private Input::InputDevice // 256
		{
			ButtonState keys[256] = {};
		};

		struct Controller : private Input::InputDevice // 128 bytes
		{
			ButtonState RUpper = {};		// 4 bytes
			ButtonState RRight = {};		// 4 bytes
			ButtonState RLower = {};		// 4 bytes
			ButtonState RLeft = {};			// 4 bytes

			ButtonState LUpper = {};		// 4 bytes
			ButtonState LRight = {};		// 4 bytes
			ButtonState LLower = {};		// 4 bytes
			ButtonState LLeft = {};			// 4 bytes

			ButtonState RShoulder = {};		// 4 bytes
			ButtonState LShoulder = {};		// 4 bytes
			ButtonState Start = {};			// 4 bytes
			ButtonState Select = {};		// 4 bytes
			
			Axis RTrigger = {};				// 8 bytes
			Axis LTrigger = {};				// 8 bytes
			
			Axis2D RJoypad = {};			// 16 bytes
			
			Axis2D LJoypad = {};			// 16 bytes

			ButtonState RJoypadButton = {};	// 4 bytes
			ButtonState LJoypadButton = {};	// 4 bytes
			byte padding[24];				// 24 bytes
		};

		LPTRACKMOUSEEVENT lpMouseTrack = {};
		alignas (64)	Mouse mouse = {};
		alignas (256)	Keyboard keyboard = {};
		alignas (128)	Controller controller[4] = {};
		Controller* _currentController = nullptr;					// 8 bytes ?
		unsigned char connectedDeviceCount = 0;						// 1 byte
		unsigned char lastConnectedDeviceCount = 0;					// 1 byte
		std::vector<bool> activeController = { false, false, false, false };
		std::vector<bool> lastActiveController = { false, false, false, false };
		XINPUT_STATE state = {};

	private:

		void UpdateXInputControllerCount();
		void GetXInput(const unsigned char controller_Index);
	};	
}
