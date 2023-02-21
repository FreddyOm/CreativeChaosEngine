#pragma once
#include "BaseManager.h"
#include "../Input/InputDevice.h"
#include "../String/String.h"

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

		void HandleWinInput(UINT msg, WPARAM wParam, LPARAM lParam);
		void HandleXInput();

	private:
		// TODO: Keep left handed mouse layout in mind
		// TODO: Store input values per bit in DWORD (or similar)
		struct Mouse : private Input::InputDevice  // 64 bytes
		{
			float deltaX;					// 4 bytes
			float deltaY;					// 4 bytes
			float lastDeltaX;				// 4 bytes
			float lastDeltay;				// 4 bytes

			float wheelDelta;				// 4 bytes
			float lastWheelDelta;			// 4 bytes
			MouseSensorState positionState; // 4 bytes
			MouseWheelState wheelState;		// 4 bytes
			
			ButtonState rightMouseButton;	// 4 bytes
			ButtonState leftMouseButton;	// 4 bytes
			ButtonState middleMouseButton;	// 4 bytes
			ButtonState extraButton1;		// 4 bytes
			
			ButtonState extraButton2;		// 4 bytes
			byte padding[12];				// 12 bytes
		};

		struct Keyboard : private Input::InputDevice
		{
			ButtonState keys[52];
		};

		struct Controller : private Input::InputDevice // 128 bytes
		{
			ButtonState RUpper;				// 4 bytes
			ButtonState RRight;				// 4 bytes
			ButtonState RLower;				// 4 bytes
			ButtonState RLeft;				// 4 bytes

			ButtonState LUpper;				// 4 bytes
			ButtonState LRight;				// 4 bytes
			ButtonState LLower;				// 4 bytes
			ButtonState LLeft;				// 4 bytes

			ButtonState RShoulder;			// 4 bytes
			ButtonState LShoulder;			// 4 bytes
			ButtonState Start;				// 4 bytes
			ButtonState Select;				// 4 bytes
			
			Axis RTrigger;					// 8 bytes
			Axis LTrigger;					// 8 bytes
			
			Joypad RJoypad;					// 16 bytes
			
			Joypad LJoypad;					// 16 bytes

			ButtonState RJoypadButton;		// 4 bytes
			ButtonState LJoypadButton;		// 4 bytes
			byte padding[24];				// 24 bytes
		};

		Mouse mouse;
		Keyboard keyboard;
		Controller controller[4];
	};	
}


