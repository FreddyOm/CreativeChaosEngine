#include "input.h"

#include "../analysis/logger.h"
#include "../analysis/debug.h"
#include "../manager/profilingManager.h"

#include "../Utilities/Math/CCMath.h"

#include "Mouse.h"
#include "Keyboard.h"
#include "Controller.h"
#include "input-device.h"

#include <vector>

#include <Xinput.h>
#pragma comment(lib, "XInput.lib")

#include "../include/ds5w/ds5w.h"

namespace CCE::Input
{

#define BUTTON_STATE InputDevice::ButtonState
#define AXIS_STATE InputDevice::AxisState
#define AXIS InputDevice::Axis

	GuiInputCallback InputCallback = nullptr;

	// InputHandler
	std::vector<Input::IInputHandler*> handlerList = {};

	alignas (64)	Mouse mouse = {};
	alignas (256)	Keyboard keyboard = {};
	alignas (128)	Controller controller[4] = {};

	Controller* _currentController = nullptr;

	unsigned char connectedDeviceCount = 0;
	unsigned char lastConnectedDeviceCount = 0;
	std::vector<bool> activeController = { false, false, false, false };
	std::vector<bool> lastActiveController = { false, false, false, false };

	// xinput
	XINPUT_STATE state = {};

	// dual sense
	DS5W::DeviceEnumInfo infos[XUSER_MAX_COUNT] = {};
	DS5W::DeviceContext con[XUSER_MAX_COUNT] = {};
	DS5W::DS5InputState inState[XUSER_MAX_COUNT] = {};
	DS5W::DS5OutputState outState[XUSER_MAX_COUNT] = {};

	Jobs::JobReturnType Initialize()
	{
		OPTICK_EVENT();
		
		DASSERT(CoInitializeEx(NULL, COINIT_MULTITHREADED) == S_OK,
			"Failed initializing COM on this thread!");

		InitializeDualSense();
	}

	Jobs::JobReturnType Deinitialize()
	{
		OPTICK_EVENT();

		CoUninitialize();
		
		handlerList.clear();

		for (int i = 0; i < XUSER_MAX_COUNT; i++)
			DS5W::freeDeviceContext(&con[i]);
	}

	Jobs::JobReturnType RegisterInputCallback(IInputHandler* handler)
	{
		handlerList.push_back(handler);
	}

	Jobs::JobReturnType UnregisterInputCallback(IInputHandler* handler)
	{
		for (auto it = handlerList.begin(); it != handlerList.end(); ++it)
		{
			if (*it == handler)
			{
				handlerList.erase(it);
			}
		}
	}

#pragma region DUAL_SENSE

	Jobs::JobReturnType InitializeDualSense()
	{
		OPTICK_EVENT();
		unsigned int dualSenseCount = 0;

		// @TODO: Maybe do this during update to get (re)connected devices
		switch (DS5W::enumDevices(infos, (unsigned int)XUSER_MAX_COUNT,
			&dualSenseCount))
		{
		default:
		{
			LOG_INPUT("NO DUAL SENSE SUPPORT YET!");
		}
		}

		for (unsigned int i = 0; i < dualSenseCount; i++)
		{
			DASSERT(DS5W::initDeviceContext(&infos[0], &con[i]) == _DS5W_ReturnValue::OK,
				"Initialization of Dual Sense device was unsuccessful!");
		}
	}

	Jobs::JobReturnType HandleDualSenseInput()
	{
		OPTICK_EVENT();
		for (DWORD controller_index = 0; controller_index < XUSER_MAX_COUNT; ++controller_index)
		{
			if (DS5W_SUCCESS(DS5W::getDeviceInputState(&con[controller_index], &inState[controller_index])))
			{
				// @TODO: Handle Dual Sense Input
			}
		}
	}

#pragma endregion DUAL_SENSE

#pragma region WIN_INPUT

	std::vector<Jobs::Job> handlerJobs{};

	Jobs::JobReturnType FinalizeWinInput()
	{
		OPTICK_EVENT();
		SAMPLE_JOB();

		// Reset values for accuracy
		mouse.deltaX = mouse.xPos - mouse.lastXPos;
		mouse.deltaY = mouse.yPos - mouse.lastYPos;

		mouse.lastXPos = mouse.xPos;
		mouse.lastYPos = mouse.yPos;
		
		// @TODO: Do this only when values change
		// @TODO: This currently doesn't really work out...

		Jobs::Counter cnt = Jobs::Counter(handlerList.size());

		for (int handlerIndex = 0; handlerIndex < handlerList.size(); ++handlerIndex)
		{
			handlerJobs.push_back(
				Jobs::JOB(UpdateInputHandler, &cnt, Jobs::Priority::NORMAL, 
					reinterpret_cast<uintptr_t>(handlerList.at(handlerIndex)))
			);

			//UpdateInputHandler(reinterpret_cast<uintptr_t>(handlerList[handlerIndex]));
		}

		Jobs::KickJobs(handlerJobs.data(), handlerList.size());
		handlerJobs.clear();

		Jobs::BusyWaitForCounter(&cnt);
	}

	Jobs::JobReturnType HandleWinInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		OPTICK_EVENT();
		SAMPLE_JOB();
		// @TODO: Implement a callback or sth that allows to call here.
		// Also check with calling over dll and stuff
		if (InputCallback != NULL && InputCallback(hWnd, msg, wParam, lParam))
			return;

		switch (msg)
		{
			// -------------------- CONFIG --------------------
		case WM_CREATE:
		{
			break;
		}

		// ------------------- KEYBOARD -------------------
		case WM_KEYDOWN:
		{
			// handle normal key down events
			if (keyboard.keys[(int)wParam] != BUTTON_STATE::PRESSED)
			{
				keyboard.keys[(int)wParam] = BUTTON_STATE::PRESSED;
				LOG_INPUT("Key down: %x", wParam);
			}
			break;
		}
		case WM_SYSKEYDOWN:
		{
			// handle system key down events
			if (keyboard.keys[(int)wParam] != BUTTON_STATE::PRESSED)
			{
				keyboard.keys[(int)wParam] = BUTTON_STATE::PRESSED;
				LOG_INPUT("SysKey down: %x", wParam);
			}
			break;
		}
		case WM_KEYUP:
		{
			// handle normal key up events
			if (keyboard.keys[(int)wParam] != BUTTON_STATE::RELEASED)
			{
				keyboard.keys[(int)wParam] = BUTTON_STATE::RELEASED;
				LOG_INPUT("Key up: %x", wParam);
			}
			break;
		}
		case WM_SYSKEYUP:
		{
			// handle system key up events
			if (keyboard.keys[(int)wParam] != BUTTON_STATE::RELEASED)
			{
				keyboard.keys[(int)wParam] = BUTTON_STATE::RELEASED;
				LOG_INPUT("SysKey up: %x", wParam);
			}
			break;
		}
		// ------------------------------------------------

		// --------------------- MOUSE --------------------
		case WM_MOUSEMOVE:
		{
			// handle mouse movement

			mouse.xPos = GET_X_LPARAM(lParam);
			mouse.yPos = GET_Y_LPARAM(lParam);

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
			mouse.wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam) / 120.f;
			mouse.lastWheelDelta = mouse.wheelDelta;
			LOG_INPUT("Mousewheel [%f]", mouse.wheelDelta);
			break;
		}
		case WM_LBUTTONDOWN:
		{
			// handle mouse button down
			if (mouse.leftMouseButton != BUTTON_STATE::PRESSED)
			{
				mouse.leftMouseButton = BUTTON_STATE::PRESSED;
				LOG_INPUT("Left Mouse button down");
			}
			break;
		}
		case WM_LBUTTONUP:
		{
			// handle mouse button up
			if (mouse.leftMouseButton != BUTTON_STATE::RELEASED)
			{
				mouse.leftMouseButton = BUTTON_STATE::RELEASED;
				LOG_INPUT("Left Mouse button up");
			}
			break;
		}
		case WM_RBUTTONDOWN:
		{
			// handle mouse button down
			if (mouse.rightMouseButton != BUTTON_STATE::PRESSED)
			{
				mouse.rightMouseButton = BUTTON_STATE::PRESSED;
				LOG_INPUT("Right Mouse button down");
			}
			break;
		}
		case WM_RBUTTONUP:
		{
			// handle mouse button up
			if (mouse.rightMouseButton != BUTTON_STATE::RELEASED)
			{
				mouse.rightMouseButton = BUTTON_STATE::RELEASED;
				LOG_INPUT("Right Mouse button up");
			}
			break;
		}
		case WM_MBUTTONDOWN:
		{
			// handle mouse button down
			if (mouse.middleMouseButton != BUTTON_STATE::PRESSED)
			{
				mouse.middleMouseButton = BUTTON_STATE::PRESSED;
				LOG_INPUT("Middle Mouse button down");
			}
			break;
		}
		case WM_MBUTTONUP:
		{
			// handle mouse button up
			if (mouse.middleMouseButton != BUTTON_STATE::RELEASED)
			{
				mouse.middleMouseButton = BUTTON_STATE::RELEASED;
				LOG_INPUT("Middle Mouse button up");
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
				if (mouse.extraButton1 != BUTTON_STATE::PRESSED)
				{
					mouse.extraButton1 = BUTTON_STATE::PRESSED;
					LOG_INPUT("Extra mouse button down");
				}
			}
			else if (button == XBUTTON2)
			{
				// XBUTTON2 was clicked.
				if (mouse.extraButton2 != BUTTON_STATE::PRESSED)
				{
					mouse.extraButton2 = BUTTON_STATE::PRESSED;
					LOG_INPUT("Extra mouse button 2 down");
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
				if (mouse.extraButton1 != BUTTON_STATE::RELEASED)
				{
					mouse.extraButton1 = BUTTON_STATE::RELEASED;
					LOG_INPUT("Extra mouse button up");
				}

			}
			else if (button == XBUTTON2)
			{
				// XBUTTON2 was released.
				if (mouse.extraButton2 != BUTTON_STATE::RELEASED)
				{
					mouse.extraButton2 = BUTTON_STATE::RELEASED;
					LOG_INPUT("Extra mouse button 2 up");
				}
			}

			break;
		}
		// ------------------------------------------------
		}
	}

	Jobs::JobReturnType UpdateInputHandler(uintptr_t inputHandler)
	{
		reinterpret_cast<IInputHandler*>(inputHandler)->InputCallback(&mouse, &keyboard, &controller[0]);
	}

#pragma endregion WIN_INPUT

#pragma region XINPUT

	Jobs::JobReturnType FetchXInputDeviceState(DWORD controllerIndex)
	{
		OPTICK_EVENT();
		OPTICK_TAG("ControllerIndex", (int)controllerIndex);
		SAMPLE_JOB();

		// Simply get the state of the _controller from XInput.
		DWORD dwResult = XInputGetState(controllerIndex, &state);

		if (dwResult == ERROR_SUCCESS)
		{
			// controller is connected
			++connectedDeviceCount;
			// maybe only check the input if state.dwPacketNumber has changed
			GetXInput(controllerIndex);
		}
		else
		{
			// controller is disconnected			
			if (activeController[controllerIndex])
			{
				activeController[controllerIndex] = false;
			}
		}
	}

	Jobs::JobReturnType HandleXInput()
	{
		OPTICK_EVENT();
		SAMPLE_JOB();

		connectedDeviceCount = 0;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		
		Jobs::Counter cnt = Jobs::Counter(XUSER_MAX_COUNT);

		Jobs::Job xInputJobs[XUSER_MAX_COUNT];

		for (DWORD controllerIndex = 0; controllerIndex < XUSER_MAX_COUNT; ++controllerIndex)
		{
			xInputJobs[controllerIndex] = Jobs::JOB(FetchXInputDeviceState, &cnt, Jobs::Priority::HIGH, controllerIndex);
		}
		
		Jobs::KickJobs(&xInputJobs[0], XUSER_MAX_COUNT);

		Jobs::BusyWaitForCounter(&cnt);
		
		if (lastConnectedDeviceCount != connectedDeviceCount) {
			UpdateXInputControllerCount();
		}

		lastConnectedDeviceCount = connectedDeviceCount;
	}

	Jobs::JobReturnType ResetInputValues()
	{
		OPTICK_EVENT();
		SAMPLE_JOB();
		mouse.wheelDelta = 0;
	}

	Jobs::JobReturnType UpdateXInputControllerCount()
	{
		OPTICK_EVENT();
		SAMPLE_JOB();

		for (unsigned short i = 0; i < XUSER_MAX_COUNT; ++i)
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

	Jobs::JobReturnType GetXInput(const DWORD controllerIndex)
	{
		OPTICK_EVENT();
		SAMPLE_JOB();
		_currentController = &controller[controllerIndex];

		if (!activeController.at(controllerIndex))
		{
			activeController.at(controllerIndex) = true;
		}

#pragma region buttons

		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			BUTTON_STATE* pButton = &_currentController->RSouth;

			if (*pButton == BUTTON_STATE::RELEASED) {
				*pButton = BUTTON_STATE::JUST_PRESSED;
				LOG_INPUT("Button [%s] pressed on Device: %i", "SOUTH", controllerIndex);
			}
			else if (*pButton == BUTTON_STATE::JUST_PRESSED)
			{
				*pButton = BUTTON_STATE::PRESSED;
			}
		}
		else
		{
			BUTTON_STATE* pButton = &_currentController->RSouth;

			if (*pButton == BUTTON_STATE::PRESSED || *pButton == BUTTON_STATE::JUST_PRESSED) {
				*pButton = BUTTON_STATE::JUST_RELEASED;
				LOG_INPUT("Button [%s] released on Device: %i", "SOUTH", controllerIndex);
			}
			else if (*pButton == BUTTON_STATE::JUST_RELEASED)
			{
				*pButton = BUTTON_STATE::RELEASED;
			}
		}

		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_B)
		{
			BUTTON_STATE* pButton = &_currentController->REast;

			if (*pButton == BUTTON_STATE::RELEASED) {
				*pButton = BUTTON_STATE::JUST_PRESSED;
				LOG_INPUT("Button [%s] pressed on Device: %i", "EAST", controllerIndex);
			}
			else if (*pButton == BUTTON_STATE::JUST_PRESSED)
			{
				*pButton = BUTTON_STATE::PRESSED;
			}
		}
		else
		{
			BUTTON_STATE* pButton = &_currentController->REast;

			if (*pButton == BUTTON_STATE::PRESSED || *pButton == BUTTON_STATE::JUST_PRESSED) {
				*pButton = BUTTON_STATE::JUST_RELEASED;
				LOG_INPUT("Button [%s] released on Device: %i", "EAST", controllerIndex);
			}
			else if (*pButton == BUTTON_STATE::JUST_RELEASED)
			{
				*pButton = BUTTON_STATE::RELEASED;
			}
		}

		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_X)
		{
			BUTTON_STATE* pButton = &_currentController->RWest;

			if (*pButton == BUTTON_STATE::RELEASED) {
				*pButton = BUTTON_STATE::JUST_PRESSED;
				LOG_INPUT("Button [%s] pressed on Device: %i", "WEST", controllerIndex);
			}
			else if (*pButton == BUTTON_STATE::JUST_PRESSED)
			{
				*pButton = BUTTON_STATE::PRESSED;
			}
		}
		else
		{
			BUTTON_STATE* pButton = &_currentController->RWest;

			if (*pButton == BUTTON_STATE::PRESSED || *pButton == BUTTON_STATE::JUST_PRESSED) {
				*pButton = BUTTON_STATE::JUST_RELEASED;
				LOG_INPUT("Button [%s] released on Device: %i", "WEST", controllerIndex);
			}
			else if (*pButton == BUTTON_STATE::JUST_RELEASED)
			{
				*pButton = BUTTON_STATE::RELEASED;
			}
		}

		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
		{
			BUTTON_STATE* pButton = &_currentController->RNorth;

			if (*pButton == BUTTON_STATE::RELEASED) {
				*pButton = BUTTON_STATE::JUST_PRESSED;
				LOG_INPUT("Button [%s] pressed on Device: %i", "NORTH", controllerIndex);
			}
			else if (*pButton == BUTTON_STATE::JUST_PRESSED)
			{
				*pButton = BUTTON_STATE::PRESSED;
			}
		}
		else
		{
			BUTTON_STATE* pButton = &_currentController->RNorth;

			if (*pButton == BUTTON_STATE::PRESSED || *pButton == BUTTON_STATE::JUST_PRESSED) {
				*pButton = BUTTON_STATE::JUST_RELEASED;
				LOG_INPUT("Button [%s] released on Device: %i", "NORTH", controllerIndex);
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
			BUTTON_STATE* pButton = &_currentController->LNorth;

			if (*pButton == BUTTON_STATE::RELEASED) {
				*pButton = BUTTON_STATE::JUST_PRESSED;
				LOG_INPUT("Button [%s] pressed on Device: %i", "DPAD UP", controllerIndex);
			}
			else if (*pButton == BUTTON_STATE::JUST_PRESSED)
			{
				*pButton = BUTTON_STATE::PRESSED;
			}
		}
		else
		{
			BUTTON_STATE* pButton = &_currentController->LNorth;

			if (*pButton == BUTTON_STATE::PRESSED || *pButton == BUTTON_STATE::JUST_PRESSED) {
				*pButton = BUTTON_STATE::JUST_RELEASED;
				LOG_INPUT("Button [%s] released on Device: %i", "DPAD UP", controllerIndex);
			}
			else if (*pButton == BUTTON_STATE::JUST_RELEASED)
			{
				*pButton = BUTTON_STATE::RELEASED;
			}
		}

		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
		{
			BUTTON_STATE* pButton = &_currentController->LEast;

			if (*pButton == BUTTON_STATE::RELEASED) {
				*pButton = BUTTON_STATE::JUST_PRESSED;
				LOG_INPUT("Button [%s] pressed on Device: %i", "DPAD RIGHT", controllerIndex);
			}
			else if (*pButton == BUTTON_STATE::JUST_PRESSED)
			{
				*pButton = BUTTON_STATE::PRESSED;
			}
		}
		else
		{
			BUTTON_STATE* pButton = &_currentController->LEast;

			if (*pButton == BUTTON_STATE::PRESSED || *pButton == BUTTON_STATE::JUST_PRESSED) {
				*pButton = BUTTON_STATE::JUST_RELEASED;
				LOG_INPUT("Button [%s] released on Device: %i", "DPAD RIGHT", controllerIndex);
			}
			else if (*pButton == BUTTON_STATE::JUST_RELEASED)
			{
				*pButton = BUTTON_STATE::RELEASED;
			}
		}

		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		{
			BUTTON_STATE* pButton = &_currentController->LSouth;

			if (*pButton == BUTTON_STATE::RELEASED) {
				*pButton = BUTTON_STATE::JUST_PRESSED;
				LOG_INPUT("Button [%s] pressed on Device: %i", "DPAD DOWN", controllerIndex);
			}
			else if (*pButton == BUTTON_STATE::JUST_PRESSED)
			{
				*pButton = BUTTON_STATE::PRESSED;
			}
		}
		else
		{
			BUTTON_STATE* pButton = &_currentController->LSouth;

			if (*pButton == BUTTON_STATE::PRESSED || *pButton == BUTTON_STATE::JUST_PRESSED) {
				*pButton = BUTTON_STATE::JUST_RELEASED;
				LOG_INPUT("Button [%s] released on Device: %i", "DPAD DOWN", controllerIndex);
			}
			else if (*pButton == BUTTON_STATE::JUST_RELEASED)
			{
				*pButton = BUTTON_STATE::RELEASED;
			}
		}

		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
		{
			BUTTON_STATE* pButton = &_currentController->LWest;

			if (*pButton == BUTTON_STATE::RELEASED) {
				*pButton = BUTTON_STATE::JUST_PRESSED;
				LOG_INPUT("Button [%s] pressed on Device: %i", "DPAD LEFT", controllerIndex);
			}
			else if (*pButton == BUTTON_STATE::JUST_PRESSED)
			{
				*pButton = BUTTON_STATE::PRESSED;
			}
		}
		else
		{
			BUTTON_STATE* pButton = &_currentController->LWest;

			if (*pButton == BUTTON_STATE::PRESSED || *pButton == BUTTON_STATE::JUST_PRESSED) {
				*pButton = BUTTON_STATE::JUST_RELEASED;
				LOG_INPUT("Button [%s] released on Device: %i", "DPAD LEFT", controllerIndex);
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
			BUTTON_STATE* pButton = &_currentController->Option2;

			if (*pButton == BUTTON_STATE::RELEASED) {
				*pButton = BUTTON_STATE::JUST_PRESSED;
				LOG_INPUT("Button [%s] pressed on Device: %i", "OPTION2", controllerIndex);
			}
			else if (*pButton == BUTTON_STATE::JUST_PRESSED)
			{
				*pButton = BUTTON_STATE::PRESSED;
			}
		}
		else
		{
			BUTTON_STATE* pButton = &_currentController->Option2;

			if (*pButton == BUTTON_STATE::PRESSED || *pButton == BUTTON_STATE::JUST_PRESSED) {
				*pButton = BUTTON_STATE::JUST_RELEASED;
				LOG_INPUT("Button [%s] released on Device: %i", "OPTION2", controllerIndex);
			}
			else if (*pButton == BUTTON_STATE::JUST_RELEASED)
			{
				*pButton = BUTTON_STATE::RELEASED;
			}
		}

		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
		{
			BUTTON_STATE* pButton = &_currentController->Option1;

			if (*pButton == BUTTON_STATE::RELEASED) {
				*pButton = BUTTON_STATE::JUST_PRESSED;
				LOG_INPUT("Button [%s] pressed on Device: %i", "OPTION1", controllerIndex);
			}
			else if (*pButton == BUTTON_STATE::JUST_PRESSED)
			{
				*pButton = BUTTON_STATE::PRESSED;
			}
		}
		else
		{
			BUTTON_STATE* pButton = &_currentController->Option1;

			if (*pButton == BUTTON_STATE::PRESSED || *pButton == BUTTON_STATE::JUST_PRESSED) {
				*pButton = BUTTON_STATE::JUST_RELEASED;
				LOG_INPUT("Button [%s] released on Device: %i", "OPTION1", controllerIndex);
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
				LOG_INPUT("Button [%s] pressed on Device: %i", "LEFT_SHOULDER", controllerIndex);
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
				LOG_INPUT("Button [%s] released on Device: %i", "LEFT_SHOULDER", controllerIndex);
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
				LOG_INPUT("Button [%s] pressed on Device: %i", "RIGHT_SHOULDER", controllerIndex);
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
				LOG_INPUT("Button [%s] released on Device: %i", "RIGHT_SHOULDER", controllerIndex);
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
				LOG_INPUT("Button [%s] pressed on Device: %i", "LEFT_THUMB", controllerIndex);
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
				LOG_INPUT("Button [%s] released on Device: %i", "LEFT_THUMB", controllerIndex);
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
				LOG_INPUT("Button [%s] pressed on Device: %i", "RIGHT_THUMB", controllerIndex);
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
				LOG_INPUT("Button [%s] released on Device: %i", "RIGHT_THUMB", controllerIndex);
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
				LOG_INPUT("Axis [%s] moved on device: %i", "LStick X", controllerIndex);
			}
			else if (pAxis->state == AXIS_STATE::AXIS_JUST_MOVED)
			{
				pAxis->state = AXIS_STATE::AXIS_MOVED;
			}
			LOG_INPUT("Axis [%s] moved on device : %i with value: %f", "LStick X", controllerIndex, pAxis->value);
		}
		else
		{
			AXIS* pAxis = &_currentController->LJoypad.x;

			if (pAxis->state == AXIS_STATE::AXIS_MOVED || pAxis->state == AXIS_STATE::AXIS_JUST_MOVED) {
				pAxis->state = AXIS_STATE::AXIS_JUST_RELEASED;
				pAxis->value = 0.0f;
				LOG_INPUT("Axis [%s] released on device: %i", "LStick Y", controllerIndex);
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
				LOG_INPUT("Axis [%s] moved on device: %i", "LStick Y", controllerIndex);
			}
			else if (pAxis->state == AXIS_STATE::AXIS_JUST_MOVED)
			{
				pAxis->state = AXIS_STATE::AXIS_MOVED;
			}
			LOG_INPUT("Axis [%s] moved on device : %i with value: %f", "LStick Y", controllerIndex, pAxis->value);
		}
		else
		{
			AXIS* pAxis = &_currentController->LJoypad.y;

			if (pAxis->state == AXIS_STATE::AXIS_MOVED || pAxis->state == AXIS_STATE::AXIS_JUST_MOVED) {
				pAxis->state = AXIS_STATE::AXIS_JUST_RELEASED;
				pAxis->value = 0.0f;
				LOG_INPUT("Axis [%s] released on device: %i", "LStick Y", controllerIndex);
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
				LOG_INPUT("Axis [%s] moved on device: %i", "RStick X", controllerIndex);
			}
			else if (pAxis->state == AXIS_STATE::AXIS_JUST_MOVED)
			{
				pAxis->state = AXIS_STATE::AXIS_MOVED;
			}
			LOG_INPUT("Axis [%s] moved on device : %i with value: %f", "RStick X", controllerIndex, pAxis->value);
		}
		else
		{
			AXIS* pAxis = &_currentController->RJoypad.x;

			if (pAxis->state == AXIS_STATE::AXIS_MOVED || pAxis->state == AXIS_STATE::AXIS_JUST_MOVED) {
				pAxis->state = AXIS_STATE::AXIS_JUST_RELEASED;
				pAxis->value = 0.0f;
				LOG_INPUT("Axis [%s] released on device: %i", "RStick X", controllerIndex);
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
				LOG_INPUT("Axis [%s] moved on device: %i", "RStick Y", controllerIndex);
			}
			else if (pAxis->state == AXIS_STATE::AXIS_JUST_MOVED)
			{
				pAxis->state = AXIS_STATE::AXIS_MOVED;
			}
			LOG_INPUT("Axis [%s] moved on device : %i with value: %f", "RStick Y", controllerIndex, pAxis->value);
		}
		else
		{
			AXIS* pAxis = &_currentController->RJoypad.y;

			if (pAxis->state == AXIS_STATE::AXIS_MOVED || pAxis->state == AXIS_STATE::AXIS_JUST_MOVED) {
				pAxis->state = AXIS_STATE::AXIS_JUST_RELEASED;
				pAxis->value = 0.0f;
				LOG_INPUT("Axis [%s] released on device: %i", "RStick Y", controllerIndex);
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
				LOG_INPUT("Axis [%s] moved on device: %i", "LTrigger", controllerIndex);
			}
			else if (pAxis->state == AXIS_STATE::AXIS_JUST_MOVED)
			{
				pAxis->state = AXIS_STATE::AXIS_MOVED;
			}

			LOG_INPUT("Axis [%s] moved on device : %i with value: %f", "LTrigger", controllerIndex, pAxis->value);
		}
		else
		{
			AXIS* pAxis = &_currentController->LTrigger;

			if (pAxis->state == AXIS_STATE::AXIS_MOVED || pAxis->state == AXIS_STATE::AXIS_JUST_MOVED) {
				pAxis->state = AXIS_STATE::AXIS_JUST_RELEASED;
				pAxis->value = 0.0f;
				LOG_INPUT("Axis [%s] released on device: %i", "LTrigger", controllerIndex);
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
				LOG_INPUT("Axis [%s] moved on device: %i", "RTrigger", controllerIndex);
			}
			else if (pAxis->state == AXIS_STATE::AXIS_JUST_MOVED)
			{
				pAxis->state = AXIS_STATE::AXIS_MOVED;
			}
			LOG_INPUT("Axis [%s] moved on device : %i with value: %f", "RTrigger", controllerIndex, pAxis->value);
		}
		else
		{
			AXIS* pAxis = &_currentController->RTrigger;

			if (pAxis->state == AXIS_STATE::AXIS_MOVED || pAxis->state == AXIS_STATE::AXIS_JUST_MOVED) {
				pAxis->state = AXIS_STATE::AXIS_JUST_RELEASED;
				pAxis->value = 0.0f;
				LOG_INPUT("Axis [%s] released on device: %i", "RTrigger", controllerIndex);
			}
			else if (pAxis->state == AXIS_STATE::AXIS_JUST_RELEASED)
			{
				pAxis->state = AXIS_STATE::AXIS_RELEASED;
			}
		}

#pragma endregion trigger
	}

#pragma endregion XINPUT

}
