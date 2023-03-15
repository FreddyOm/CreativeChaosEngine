#include "InputManager.h"
#include "../Analysis/Logger.h"
#include "../Analysis/Debug.h"
#include "../Analysis/Time.h"
#include "../Utilities/Math/CCMath.h"

#define BUTTON_STATE CCE::Input::InputDevice::ButtonState
#define AXIS_STATE CCE::Input::InputDevice::AxisState
#define AXIS CCE::Input::InputDevice::Axis

namespace CCE
{
	/// <summary>
	/// The startup call for the manager. Initializes the manager.
	/// </summary>
	void CCE::InputManager::StartUp()
	{
		DASSERT(Instance == nullptr, "InputManager was instantiated more than once!");
		Instance = this;

		auto startTime = Time::CurrentTick();
		CoInitializeEx(NULL, COINIT_MULTITHREADED);
		initialized = true;

		auto endTime = Time::CurrentTick();
		double initDuration = Time::GetDurationInMicroSec(startTime, endTime);
		LOGC("InputManager initialized!", COLOR_BLUE);
	}

	/// <summary>
	/// The shutdown call of the manager. Deinitializes all contents of the manager.
	/// </summary>
	void CCE::InputManager::ShutDown()
	{
		delete lpMouseTrack;
		LOGC("Shutting down InputManager...", COLOR_BLUE);
		initialized = false;
		Instance = nullptr;
	}

	/// <summary>
	/// A singelton instance pointer that points to itself.
	/// </summary>
	InputManager* InputManager::Instance = nullptr;

#ifdef CCE_PLATFORM_WINDOWS // PLATFORM WINDOWS
	/// <summary>
	/// Handle window input (Mouse + Keyoard).
	/// </summary>
	/// <param name="msg">The current message type.</param>
	/// <param name="wParam">The high word parameter.</param>
	/// <param name="lParam">The low word parameter.</param>
	void InputManager::HandleWinInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		// -------------------- CONFIG --------------------
		case WM_CREATE:
		{
			// Track additional mouse info
			//TODO: Remove new and allocate memory in custom memalloc
			lpMouseTrack = new TRACKMOUSEEVENT();
			
			lpMouseTrack->cbSize = sizeof(TRACKMOUSEEVENT);
			lpMouseTrack->dwFlags = TME_HOVER, TME_LEAVE;
			lpMouseTrack->dwHoverTime = HOVER_DEFAULT;
			lpMouseTrack->hwndTrack = hWnd;

			break;
		}

		// ------------------- KEYBOARD -------------------
		case WM_KEYDOWN:
		{
			// handle normal key down events
			if (keyboard.keys[(int)wParam] != BUTTON_STATE::PRESSED)
			{
				keyboard.keys[(int)wParam] =
					keyboard.keys[(int)wParam] == BUTTON_STATE::JUST_PRESSED ?
					BUTTON_STATE::PRESSED : BUTTON_STATE::JUST_PRESSED;

				if (keyboard.keys[(int)wParam] == BUTTON_STATE::JUST_PRESSED)
				{
					LOG_INPUT("Key down: %x", wParam);
				}
			}			
			break;
		}
		case WM_SYSKEYDOWN:
		{
			// handle system key down events
			if (keyboard.keys[(int)wParam] != BUTTON_STATE::PRESSED)
			{
				keyboard.keys[(int)wParam] =
					keyboard.keys[(int)wParam] == BUTTON_STATE::JUST_PRESSED ?
					BUTTON_STATE::PRESSED : BUTTON_STATE::JUST_PRESSED;

				if (keyboard.keys[(int)wParam] == BUTTON_STATE::JUST_PRESSED)
				{
					LOG_INPUT("SysKey down: %x", wParam);
				}
			}
			break;
		}
		case WM_KEYUP:
		{
			// handle normal key up events
			if (keyboard.keys[(int)wParam] != BUTTON_STATE::RELEASED)
			{
				keyboard.keys[(int)wParam] =
					keyboard.keys[(int)wParam] == BUTTON_STATE::JUST_RELEASED ?
					BUTTON_STATE::RELEASED : BUTTON_STATE::JUST_RELEASED;

				if (keyboard.keys[(int)wParam] == BUTTON_STATE::JUST_RELEASED)
				{
					LOG_INPUT("Key up: %x", wParam);
				}
			}
			break;
		}
		case WM_SYSKEYUP:
		{
			// handle system key up events
			if (keyboard.keys[(int)wParam] != BUTTON_STATE::RELEASED)
			{
				keyboard.keys[(int)wParam] =
					keyboard.keys[(int)wParam] == BUTTON_STATE::JUST_RELEASED ?
					BUTTON_STATE::RELEASED : BUTTON_STATE::JUST_RELEASED;

				if (keyboard.keys[(int)wParam] == BUTTON_STATE::JUST_RELEASED)
				{
					LOG_INPUT("SysKey up: %x", wParam);
				}
			}
			break;
		}
		// ------------------------------------------------
		
		// --------------------- MOUSE --------------------
		case WM_MOUSEMOVE:
		{			
			// handle mouse movement
			mouse.lastXPos = mouse.xPos;
			mouse.lastYPos = mouse.yPos;

			mouse.deltaX = mouse.lastXPos - mouse.xPos;
			mouse.deltaY = mouse.lastYPos - mouse.yPos;

			mouse.xPos = GET_X_LPARAM(lParam);
			mouse.yPos = GET_Y_LPARAM(lParam);

			DASSERT(TrackMouseEvent(lpMouseTrack), "Tracking mouse events unsuccessful!");

			LOG_INPUT("Mouse position [x: %i y: %i]", mouse.xPos, mouse.yPos);
			break;
		}
		case WM_MOUSELEAVE:
		{
			// handle mouse leave event
			break;
		}
		case WM_NCMOUSELEAVE:
		{
			// handle mouse leave event
			break;
		}
		case WM_MOUSEHOVER:
		{
			// handle mouse enter event
			break;
		}
		case WM_NCMOUSEHOVER:
		{
			// handle mouse enter event
			break;
		}
		case WM_MOUSEWHEEL:
		{
			mouse.lastWheelDelta = mouse.wheelDelta;
			mouse.wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			LOG_INPUT("Mousewheel [%f]", mouse.wheelDelta);
			break;
		}
		case WM_LBUTTONDOWN:
		{
			// handle mouse button down
			if (mouse.leftMouseButton != BUTTON_STATE::PRESSED &&
				mouse.leftMouseButton != BUTTON_STATE::JUST_PRESSED)
			{
				mouse.leftMouseButton =
					mouse.leftMouseButton == BUTTON_STATE::JUST_PRESSED ?
					BUTTON_STATE::PRESSED :
					BUTTON_STATE::JUST_PRESSED;

				if (mouse.leftMouseButton == BUTTON_STATE::JUST_PRESSED)
				{
					LOG_INPUT("Left Mouse button down");
				}
			}
			break;
		}
		case WM_LBUTTONUP:
		{
			// handle mouse button up
			if (mouse.leftMouseButton != BUTTON_STATE::RELEASED &&
				mouse.leftMouseButton != BUTTON_STATE::JUST_RELEASED)
			{
				mouse.leftMouseButton =
					mouse.leftMouseButton == BUTTON_STATE::JUST_RELEASED ?
					BUTTON_STATE::RELEASED :
					BUTTON_STATE::JUST_RELEASED;

				if (mouse.leftMouseButton == BUTTON_STATE::JUST_RELEASED)
				{
					LOG_INPUT("Left Mouse button up");
				}
			}
			break;
		}
		case WM_RBUTTONDOWN:
		{
			// handle mouse button down
			if (mouse.rightMouseButton != BUTTON_STATE::PRESSED &&
				mouse.rightMouseButton != BUTTON_STATE::JUST_PRESSED)
			{
				mouse.rightMouseButton =
					mouse.rightMouseButton == BUTTON_STATE::JUST_PRESSED ?
					BUTTON_STATE::PRESSED :
					BUTTON_STATE::JUST_PRESSED;

				if (mouse.rightMouseButton == BUTTON_STATE::JUST_PRESSED)
				{
					LOG_INPUT("Right Mouse button down");
				}
			}			
			break;
		}
		case WM_RBUTTONUP:
		{
			// handle mouse button up
			if (mouse.rightMouseButton != BUTTON_STATE::RELEASED &&
				mouse.rightMouseButton != BUTTON_STATE::JUST_RELEASED)
			{
				mouse.rightMouseButton =
					mouse.rightMouseButton == BUTTON_STATE::JUST_RELEASED ?
					BUTTON_STATE::RELEASED :
					BUTTON_STATE::JUST_RELEASED;

				if (mouse.rightMouseButton == BUTTON_STATE::JUST_RELEASED)
				{
					LOG_INPUT("Right Mouse button up");
				}
			}
			break;
		}
		case WM_MBUTTONDOWN:
		{
			// handle mouse button down
			if (mouse.middleMouseButton != BUTTON_STATE::PRESSED &&
				mouse.middleMouseButton != BUTTON_STATE::JUST_PRESSED)
			{
				mouse.middleMouseButton =
					mouse.middleMouseButton == BUTTON_STATE::JUST_PRESSED ?
					BUTTON_STATE::PRESSED :
					BUTTON_STATE::JUST_PRESSED;

				if (mouse.middleMouseButton == BUTTON_STATE::JUST_PRESSED)
				{
					LOG_INPUT("Middle Mouse button down");
				}
			}
			break;
		}
		case WM_MBUTTONUP:
		{
			// handle mouse button up
			if (mouse.middleMouseButton != BUTTON_STATE::RELEASED &&
				mouse.middleMouseButton != BUTTON_STATE::JUST_RELEASED)
			{
				mouse.middleMouseButton =
					mouse.middleMouseButton == BUTTON_STATE::JUST_RELEASED ?
					BUTTON_STATE::RELEASED :
					BUTTON_STATE::JUST_RELEASED;

				if (mouse.middleMouseButton == BUTTON_STATE::JUST_RELEASED)
				{
					LOG_INPUT("Middle Mouse button up");
				}
			}
			break;
		}
		case WM_XBUTTONDOWN:
		{
			// handle mouse button down

			UINT button = GET_XBUTTON_WPARAM(wParam);
			
			if (button == XBUTTON1)
			{
				// XBUTTON1 was clicked.
				if (mouse.extraButton1 != BUTTON_STATE::PRESSED &&
					mouse.extraButton1 != BUTTON_STATE::JUST_PRESSED)
				{
					mouse.extraButton1 =
						mouse.extraButton1 == BUTTON_STATE::JUST_PRESSED ?
						BUTTON_STATE::PRESSED :
						BUTTON_STATE::JUST_PRESSED;

					if (mouse.extraButton1 == BUTTON_STATE::JUST_PRESSED)
					{
						LOG_INPUT("Extra mouse button down");
					}
				}
			}
			else if (button == XBUTTON2)
			{
				// XBUTTON2 was clicked.
				if (mouse.extraButton2 != BUTTON_STATE::PRESSED &&
					mouse.extraButton2 != BUTTON_STATE::JUST_PRESSED)
				{
					mouse.extraButton2 =
						mouse.extraButton2 == BUTTON_STATE::JUST_PRESSED ?
						BUTTON_STATE::PRESSED :
						BUTTON_STATE::JUST_PRESSED;

					if (mouse.extraButton2 == BUTTON_STATE::JUST_PRESSED)
					{
						LOG_INPUT("Extra mouse button 2 down");
					}
				}				
			}

			break;
		}
		case WM_XBUTTONUP:
		{
			// handle mouse button up

			UINT button = GET_XBUTTON_WPARAM(wParam);

			if (button == XBUTTON1)
			{
				// XBUTTON1 was released.
				if (mouse.extraButton1 != BUTTON_STATE::RELEASED &&
					mouse.extraButton1 != BUTTON_STATE::JUST_RELEASED)
				{
					mouse.extraButton1 =
						mouse.extraButton1 == BUTTON_STATE::JUST_RELEASED ? 
						BUTTON_STATE::RELEASED :
						BUTTON_STATE::JUST_RELEASED;
				}
				if (mouse.extraButton1 == BUTTON_STATE::JUST_RELEASED)
				{
					LOG_INPUT("Extra mouse button up");
				}
			}
			else if (button == XBUTTON2)
			{
				// XBUTTON2 was released.
				if (mouse.extraButton2 != BUTTON_STATE::RELEASED &&
					mouse.extraButton2 != BUTTON_STATE::JUST_RELEASED)
				{
					mouse.extraButton2 =
						mouse.extraButton2 == BUTTON_STATE::JUST_RELEASED ?
						BUTTON_STATE::RELEASED :
						BUTTON_STATE::JUST_RELEASED;
				}
				if (mouse.extraButton2 == BUTTON_STATE::JUST_RELEASED)
				{
					LOG_INPUT("Extra mouse button 2 up");
				}
				
			}

			break;
		}
		// ------------------------------------------------
		}
		// reset value
		mouse.wheelDelta = 0;
	}
	
	/// <summary>
	/// Handle XInput (Controller).
	/// </summary>
	void InputManager::HandleXInput()
	{
		connectedDeviceCount = 0;

		DWORD dwResult;
		for (DWORD controller_index = 0; controller_index < XUSER_MAX_COUNT; controller_index++)
		{
			ZeroMemory(&state, sizeof(XINPUT_STATE));

			// Simply get the state of the _controller from XInput.
			dwResult = XInputGetState(controller_index, &state);

			if (dwResult == ERROR_SUCCESS)
			{
				// controller is connected
				connectedDeviceCount++;
				// maybe only check the input if state.dwPacketNumber has changed
				GetXInput(controller_index);
			}
			else
			{
				// controller is disconnected			
				if (activeController[controller_index])
				{
					activeController[controller_index] = false;
				}
			}
		}

		if (lastConnectedDeviceCount != connectedDeviceCount) {
			UpdateXInputControllerCount();
		}

		lastConnectedDeviceCount = connectedDeviceCount;
	}

	/// <summary>
	/// Handle DirectInput (Controller).
	/// </summary>
	void InputManager::HandleDirectInput()
	{
		// TODO: Handle Direct Input ?

	}

	/// <summary>
	/// Updates the controller count and sends connected / disconnected events
	/// </summary>
	void InputManager::UpdateXInputControllerCount()
	{
		// TODO: Implement
		// Do stuff here when Controller was connected / disconnected
		for (unsigned short i = 0; i < XUSER_MAX_COUNT; i++)
		{
			if (activeController[i] != lastActiveController[i])
			{
				if (activeController[i])
				{
					LOG_INPUT("Controller %i connected", i);
				}
				else 
				{
					LOG_INPUT("Controller %i disconnected", i);
				}
			}

			lastActiveController[i] = activeController[i];
		}
	}

	/// <summary>
	/// Collects the actual input data.
	/// </summary>
	/// <param name="controller_Index"></param>
	void InputManager::GetXInput(const unsigned char controller_Index)
	{
		_currentController = &controller[controller_Index];

		if (!activeController.at(controller_Index))
		{
			activeController.at(controller_Index) = true;
		}

#pragma region buttons

		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			BUTTON_STATE* pButton = &_currentController->RLower;

			if (*pButton == BUTTON_STATE::RELEASED) {
				*pButton = BUTTON_STATE::JUST_PRESSED;
				LOG_INPUT("Button [%s] pressed on Device: %i" ,"A", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_PRESSED)
			{
				*pButton = BUTTON_STATE::PRESSED;
			}
		}
		else
		{
			BUTTON_STATE* pButton = &_currentController->RLower;

			if (*pButton == BUTTON_STATE::PRESSED || *pButton == BUTTON_STATE::JUST_PRESSED) {
				*pButton = BUTTON_STATE::JUST_RELEASED;
				LOG_INPUT("Button [%s] released on Device: %i", "A", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_RELEASED)
			{
				*pButton = BUTTON_STATE::RELEASED;
			}
		}

		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_B)
		{
			BUTTON_STATE* pButton = &_currentController->RRight;

			if (*pButton == BUTTON_STATE::RELEASED) {
				*pButton = BUTTON_STATE::JUST_PRESSED;
				LOG_INPUT("Button [%s] pressed on Device: %i", "B", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_PRESSED)
			{
				*pButton = BUTTON_STATE::PRESSED;
			}
		}
		else
		{
			BUTTON_STATE* pButton = &_currentController->RRight;

			if (*pButton == BUTTON_STATE::PRESSED || *pButton == BUTTON_STATE::JUST_PRESSED) {
				*pButton = BUTTON_STATE::JUST_RELEASED;
				LOG_INPUT("Button [%s] released on Device: %i", "B", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_RELEASED)
			{
				*pButton = BUTTON_STATE::RELEASED;
			}
		}

		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_X)
		{
			BUTTON_STATE* pButton = &_currentController->RLeft;

			if (*pButton == BUTTON_STATE::RELEASED) {
				*pButton = BUTTON_STATE::JUST_PRESSED;
				LOG_INPUT("Button [%s] pressed on Device: %i", "X", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_PRESSED)
			{
				*pButton = BUTTON_STATE::PRESSED;
			}
		}
		else
		{
			BUTTON_STATE* pButton = &_currentController->RLeft;

			if (*pButton == BUTTON_STATE::PRESSED || *pButton == BUTTON_STATE::JUST_PRESSED) {
				*pButton = BUTTON_STATE::JUST_RELEASED;
				LOG_INPUT("Button [%s] released on Device: %i", "X", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_RELEASED)
			{
				*pButton = BUTTON_STATE::RELEASED;
			}
		}

		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
		{
			BUTTON_STATE* pButton = &_currentController->RUpper;

			if (*pButton == BUTTON_STATE::RELEASED) {
				*pButton = BUTTON_STATE::JUST_PRESSED;
				LOG_INPUT("Button [%s] pressed on Device: %i", "Y", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_PRESSED)
			{
				*pButton = BUTTON_STATE::PRESSED;
			}
		}
		else
		{
			BUTTON_STATE* pButton = &_currentController->RUpper;

			if (*pButton == BUTTON_STATE::PRESSED || *pButton == BUTTON_STATE::JUST_PRESSED) {
				*pButton = BUTTON_STATE::JUST_RELEASED;
				LOG_INPUT("Button [%s] released on Device: %i", "Y", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_RELEASED)
			{
				*pButton = BUTTON_STATE::RELEASED;
			}
		}

#pragma endregion buttons

#pragma region dpad

		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
		{
			BUTTON_STATE* pButton = &_currentController->LUpper;

			if (*pButton == BUTTON_STATE::RELEASED) {
				*pButton = BUTTON_STATE::JUST_PRESSED;
				LOG_INPUT("Button [%s] pressed on Device: %i" ,"DPAD UP", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_PRESSED)
			{
				*pButton = BUTTON_STATE::PRESSED;
			}
	}
		else
		{
			BUTTON_STATE* pButton = &_currentController->LUpper;

			if (*pButton == BUTTON_STATE::PRESSED || *pButton == BUTTON_STATE::JUST_PRESSED) {
				*pButton = BUTTON_STATE::JUST_RELEASED;
				LOG_INPUT("Button [%s] released on Device: %i", "DPAD UP", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_RELEASED)
			{
				*pButton = BUTTON_STATE::RELEASED;
			}
		}

		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
		{
			BUTTON_STATE* pButton = &_currentController->LRight;

			if (*pButton == BUTTON_STATE::RELEASED) {
				*pButton = BUTTON_STATE::JUST_PRESSED;
				LOG_INPUT("Button [%s] pressed on Device: %i" ,"DPAD RIGHT", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_PRESSED)
			{
				*pButton = BUTTON_STATE::PRESSED;
			}
		}
		else
		{
			BUTTON_STATE* pButton = &_currentController->LRight;

			if (*pButton == BUTTON_STATE::PRESSED || *pButton == BUTTON_STATE::JUST_PRESSED) {
				*pButton = BUTTON_STATE::JUST_RELEASED;
				LOG_INPUT("Button [%s] released on Device: %i", "DPAD RIGHT", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_RELEASED)
			{
				*pButton = BUTTON_STATE::RELEASED;
			}
		}

		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		{
			BUTTON_STATE* pButton = &_currentController->LLower;

			if (*pButton == BUTTON_STATE::RELEASED) {
				*pButton = BUTTON_STATE::JUST_PRESSED;
				LOG_INPUT("Button [%s] pressed on Device: %i" ,"DPAD DOWN", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_PRESSED)
			{
				*pButton = BUTTON_STATE::PRESSED;
			}
		}
		else
		{
			BUTTON_STATE* pButton = &_currentController->LLower;

			if (*pButton == BUTTON_STATE::PRESSED || *pButton == BUTTON_STATE::JUST_PRESSED) {
				*pButton = BUTTON_STATE::JUST_RELEASED;
				LOG_INPUT("Button [%s] released on Device: %i", "DPAD DOWN", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_RELEASED)
			{
				*pButton = BUTTON_STATE::RELEASED;
			}
		}

		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
		{
			BUTTON_STATE* pButton = &_currentController->LLeft;

			if (*pButton == BUTTON_STATE::RELEASED) {
				*pButton = BUTTON_STATE::JUST_PRESSED;
				LOG_INPUT("Button [%s] pressed on Device: %i" ,"DPAD LEFT", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_PRESSED)
			{
				*pButton = BUTTON_STATE::PRESSED;
			}
		}
		else
		{
			BUTTON_STATE* pButton = &_currentController->LLeft;

			if (*pButton == BUTTON_STATE::PRESSED || *pButton == BUTTON_STATE::JUST_PRESSED) {
				*pButton = BUTTON_STATE::JUST_RELEASED;
				LOG_INPUT("Button [%s] released on Device: %i", "DPAD LEFT", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_RELEASED)
			{
				*pButton = BUTTON_STATE::RELEASED;
			}
		}

#pragma endregion dpad

#pragma region options

		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_START)
		{
			BUTTON_STATE* pButton = &_currentController->Start;

			if (*pButton == BUTTON_STATE::RELEASED) {
				*pButton = BUTTON_STATE::JUST_PRESSED;
				LOG_INPUT("Button [%s] pressed on Device: %i" ,"START", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_PRESSED)
			{
				*pButton = BUTTON_STATE::PRESSED;
			}
		}
		else
		{
			BUTTON_STATE* pButton = &_currentController->Start;

			if (*pButton == BUTTON_STATE::PRESSED || *pButton == BUTTON_STATE::JUST_PRESSED) {
				*pButton = BUTTON_STATE::JUST_RELEASED;
				LOG_INPUT("Button [%s] released on Device: %i", "START", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_RELEASED)
			{
				*pButton = BUTTON_STATE::RELEASED;
			}
		}

		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
		{
			BUTTON_STATE* pButton = &_currentController->Select;

			if (*pButton == BUTTON_STATE::RELEASED) {
				*pButton = BUTTON_STATE::JUST_PRESSED;
				LOG_INPUT("Button [%s] pressed on Device: %i" ,"BACK", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_PRESSED)
			{
				*pButton = BUTTON_STATE::PRESSED;
			}
		}
		else
		{
			BUTTON_STATE* pButton = &_currentController->Select;

			if (*pButton == BUTTON_STATE::PRESSED || *pButton == BUTTON_STATE::JUST_PRESSED) {
				*pButton = BUTTON_STATE::JUST_RELEASED;
				LOG_INPUT("Button [%s] released on Device: %i", "BACK", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_RELEASED)
			{
				*pButton = BUTTON_STATE::RELEASED;
			}
		}

#pragma endregion options

#pragma region soulder buttons

		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			BUTTON_STATE* pButton = &_currentController->LShoulder;

			if (*pButton == BUTTON_STATE::RELEASED) {
				*pButton = BUTTON_STATE::JUST_PRESSED;
				LOG_INPUT("Button [%s] pressed on Device: %i" ,"LEFT_SHOULDER", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_PRESSED)
			{
				*pButton = BUTTON_STATE::PRESSED;
			}
		}
		else
		{
			BUTTON_STATE* pButton = &_currentController->LShoulder;

			if (*pButton == BUTTON_STATE::PRESSED || *pButton == BUTTON_STATE::JUST_PRESSED) {
				*pButton = BUTTON_STATE::JUST_RELEASED;
				LOG_INPUT("Button [%s] released on Device: %i", "LEFT_SHOULDER", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_RELEASED)
			{
				*pButton = BUTTON_STATE::RELEASED;
			}
		}

		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			BUTTON_STATE* pButton = &_currentController->RShoulder;

			if (*pButton == BUTTON_STATE::RELEASED) {
				*pButton = BUTTON_STATE::JUST_PRESSED;
				LOG_INPUT("Button [%s] pressed on Device: %i" ,"RIGHT_SHOULDER", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_PRESSED)
			{
				*pButton = BUTTON_STATE::PRESSED;
			}
		}
		else
		{
			BUTTON_STATE* pButton = &_currentController->RShoulder;

			if (*pButton == BUTTON_STATE::PRESSED || *pButton == BUTTON_STATE::JUST_PRESSED) {
				*pButton = BUTTON_STATE::JUST_RELEASED;
				LOG_INPUT("Button [%s] released on Device: %i", "RIGHT_SHOULDER", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_RELEASED)
			{
				*pButton = BUTTON_STATE::RELEASED;
			}
		}

#pragma endregion soulder buttons

#pragma region joystick buttons

		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
		{
			BUTTON_STATE* pButton = &_currentController->LJoypadButton;

			if (*pButton == BUTTON_STATE::RELEASED) {
				*pButton = BUTTON_STATE::JUST_PRESSED;
				LOG_INPUT("Button [%s] pressed on Device: %i" ,"LEFT_THUMB", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_PRESSED)
			{
				*pButton = BUTTON_STATE::PRESSED;
			}
		}
		else
		{
			BUTTON_STATE* pButton = &_currentController->LJoypadButton;

			if (*pButton == BUTTON_STATE::PRESSED || *pButton == BUTTON_STATE::JUST_PRESSED) {
				*pButton = BUTTON_STATE::JUST_RELEASED;
				LOG_INPUT("Button [%s] released on Device: %i", "LEFT_THUMB", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_RELEASED)
			{
				*pButton = BUTTON_STATE::RELEASED;
			}
		}

		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
		{
			BUTTON_STATE* pButton = &_currentController->RJoypadButton;

			if (*pButton == BUTTON_STATE::RELEASED) {
				*pButton = BUTTON_STATE::JUST_PRESSED;
				LOG_INPUT("Button [%s] pressed on Device: %i" ,"RIGHT_THUMB", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_PRESSED)
			{
				*pButton = BUTTON_STATE::PRESSED;
			}
		}
		else
		{
			BUTTON_STATE* pButton = &_currentController->RJoypadButton;

			if (*pButton == BUTTON_STATE::PRESSED || *pButton == BUTTON_STATE::JUST_PRESSED) {
				*pButton = BUTTON_STATE::JUST_RELEASED;
				LOG_INPUT("Button [%s] released on Device: %i", "RIGHT_THUMB", controller_Index);
			}
			else if (*pButton == BUTTON_STATE::JUST_RELEASED)
			{
				*pButton = BUTTON_STATE::RELEASED;
			}
		}

#pragma endregion joystick buttons

#pragma region stick axis

		// left stick

		if (state.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
			state.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			AXIS* pAxis = &_currentController->LJoypad.x;

			pAxis->value = Math::CCMath::Clamp((float)state.Gamepad.sThumbLX / 32767.0f, -1.0f, 1.0f);

			if (pAxis->state == AXIS_STATE::AXIS_RELEASED) {
				pAxis->state = AXIS_STATE::AXIS_JUST_MOVED;
				LOG_INPUT("Axis [%s] moved on device: %i", "LStick X", controller_Index);
			}
			else if (pAxis->state == AXIS_STATE::AXIS_JUST_MOVED)
			{
				pAxis->state = AXIS_STATE::AXIS_MOVED;
			}
			LOG_INPUT("Axis [%s] moved on device : %i with value: %f", "LStick X", controller_Index, pAxis->value);
		}
		else
		{
			AXIS* pAxis = &_currentController->LJoypad.x;

			if (pAxis->state == AXIS_STATE::AXIS_MOVED || pAxis->state == AXIS_STATE::AXIS_JUST_MOVED) {
				pAxis->state = AXIS_STATE::AXIS_JUST_RELEASED;
				pAxis->value = 0.0f;
				LOG_INPUT("Axis [%s] released on device: %i", "LStick Y", controller_Index);
			}
			else if (pAxis->state == AXIS_STATE::AXIS_JUST_RELEASED)
			{
				pAxis->state = AXIS_STATE::AXIS_RELEASED;
			}
		}

		if (state.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
			state.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			AXIS* pAxis = &_currentController->LJoypad.y;

			pAxis->value = Math::CCMath::Clamp((float)state.Gamepad.sThumbLY / 32767.0f, -1.0f, 1.0f);

			if (pAxis->state == AXIS_STATE::AXIS_RELEASED) {
				pAxis->state = AXIS_STATE::AXIS_JUST_MOVED;
				LOG_INPUT("Axis [%s] moved on device: %i" , "LStick Y", controller_Index);
			}
			else if (pAxis->state == AXIS_STATE::AXIS_JUST_MOVED)
			{
				pAxis->state = AXIS_STATE::AXIS_MOVED;
			}
			LOG_INPUT("Axis [%s] moved on device : %i with value: %f", "LStick Y", controller_Index, pAxis->value);
		}
		else
		{
			AXIS* pAxis = &_currentController->LJoypad.y;

			if (pAxis->state == AXIS_STATE::AXIS_MOVED || pAxis->state == AXIS_STATE::AXIS_JUST_MOVED) {
				pAxis->state = AXIS_STATE::AXIS_JUST_RELEASED;
				pAxis->value = 0.0f;
				LOG_INPUT("Axis [%s] released on device: %i", "LStick Y", controller_Index);
			}
			else if (pAxis->state == AXIS_STATE::AXIS_JUST_RELEASED)
			{
				pAxis->state = AXIS_STATE::AXIS_RELEASED;
			}
		}

		// right stick

		if (state.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
			state.Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			AXIS* pAxis = &_currentController->RJoypad.x;

			pAxis->value = Math::CCMath::Clamp((float)state.Gamepad.sThumbRX / 32767.0f, -1.0f, 1.0f);

			if (pAxis->state == AXIS_STATE::AXIS_RELEASED) {
				pAxis->state = AXIS_STATE::AXIS_JUST_MOVED;
				LOG_INPUT("Axis [%s] moved on device: %i" , "RStick X", controller_Index);
			}
			else if (pAxis->state == AXIS_STATE::AXIS_JUST_MOVED)
			{
				pAxis->state = AXIS_STATE::AXIS_MOVED;
			}
			LOG_INPUT("Axis [%s] moved on device : %i with value: %f", "RStick X", controller_Index, pAxis->value);
		}
		else
		{
			AXIS* pAxis = &_currentController->RJoypad.x;

			if (pAxis->state == AXIS_STATE::AXIS_MOVED || pAxis->state == AXIS_STATE::AXIS_JUST_MOVED) {
				pAxis->state = AXIS_STATE::AXIS_JUST_RELEASED;
				pAxis->value = 0.0f;
				LOG_INPUT("Axis [%s] released on device: %i", "RStick X", controller_Index);
			}
			else if (pAxis->state == AXIS_STATE::AXIS_JUST_RELEASED)
			{
				pAxis->state = AXIS_STATE::AXIS_RELEASED;
			}
		}

		if (state.Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
			state.Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			AXIS* pAxis = &_currentController->RJoypad.y;

			pAxis->value = Math::CCMath::Clamp((float)state.Gamepad.sThumbRY / 32767.0f, -1.0f, 1.0f);

			if (pAxis->state == AXIS_STATE::AXIS_RELEASED) {
				pAxis->state = AXIS_STATE::AXIS_JUST_MOVED;
				LOG_INPUT("Axis [%s] moved on device: %i" , "RStick Y", controller_Index);
			}
			else if (pAxis->state == AXIS_STATE::AXIS_JUST_MOVED)
			{
				pAxis->state = AXIS_STATE::AXIS_MOVED;
			}
			LOG_INPUT("Axis [%s] moved on device : %i with value: %f", "RStick Y", controller_Index, pAxis->value);
		}
		else
		{
			AXIS* pAxis = &_currentController->RJoypad.y;

			if (pAxis->state == AXIS_STATE::AXIS_MOVED || pAxis->state == AXIS_STATE::AXIS_JUST_MOVED) {
				pAxis->state = AXIS_STATE::AXIS_JUST_RELEASED;
				pAxis->value = 0.0f;
				LOG_INPUT("Axis [%s] released on device: %i", "RStick Y", controller_Index);
			}
			else if (pAxis->state == AXIS_STATE::AXIS_JUST_RELEASED)
			{
				pAxis->state = AXIS_STATE::AXIS_RELEASED;
			}
		}

#pragma endregion stick axis

#pragma region trigger

		// left trigger

		if (state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			AXIS* pAxis = &_currentController->LTrigger;

			pAxis->value = Math::CCMath::Clamp01((float)state.Gamepad.bLeftTrigger / 255.0f);

			if (pAxis->state == AXIS_STATE::AXIS_RELEASED) {
				pAxis->state = AXIS_STATE::AXIS_JUST_MOVED;
				LOG_INPUT("Axis [%s] moved on device: %i" , "LTrigger", controller_Index);
			}
			else if (pAxis->state == AXIS_STATE::AXIS_JUST_MOVED)
			{
				pAxis->state = AXIS_STATE::AXIS_MOVED;
			}

			LOG_INPUT("Axis [%s] moved on device : %i with value: %f", "LTrigger", controller_Index, pAxis->value);
		}
		else
		{
			AXIS* pAxis = &_currentController->LTrigger;

			if (pAxis->state == AXIS_STATE::AXIS_MOVED || pAxis->state == AXIS_STATE::AXIS_JUST_MOVED) {
				pAxis->state = AXIS_STATE::AXIS_JUST_RELEASED;
				pAxis->value = 0.0f;
				LOG_INPUT("Axis [%s] released on device: %i", "LTrigger", controller_Index);
			}
			else if (pAxis->state == AXIS_STATE::AXIS_JUST_RELEASED)
			{
				pAxis->state = AXIS_STATE::AXIS_RELEASED;
			}
		}


		// right trigger

		if (state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			AXIS* pAxis = &_currentController->RTrigger;

			pAxis->value = Math::CCMath::Clamp01((float)state.Gamepad.bRightTrigger / 255.0f);

			if (pAxis->state == AXIS_STATE::AXIS_RELEASED) {
				pAxis->state = AXIS_STATE::AXIS_JUST_MOVED;
				LOG_INPUT("Axis [%s] moved on device: %i" , "RTrigger", controller_Index);
			}
			else if (pAxis->state == AXIS_STATE::AXIS_JUST_MOVED)
			{
				pAxis->state = AXIS_STATE::AXIS_MOVED;
			}
			LOG_INPUT("Axis [%s] moved on device : %i with value: %f", "RTrigger", controller_Index, pAxis->value);
		}
		else
		{
			AXIS* pAxis = &_currentController->RTrigger;

			if (pAxis->state == AXIS_STATE::AXIS_MOVED || pAxis->state == AXIS_STATE::AXIS_JUST_MOVED) {
				pAxis->state = AXIS_STATE::AXIS_JUST_RELEASED;
				pAxis->value = 0.0f;
				LOG_INPUT("Axis [%s] released on device: %i", "RTrigger", controller_Index);
			}
			else if (pAxis->state == AXIS_STATE::AXIS_JUST_RELEASED)
			{
				pAxis->state = AXIS_STATE::AXIS_RELEASED;
			}
		}

#pragma endregion trigger
	}

#else
#error CCE is currently only supported for Windows
#endif // CCE_PLATFORM_WINDOWS
}
