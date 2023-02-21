#include "InputManager.h"
#include "../Analysis/Logger.h"
#include "../Analysis/Debug.h"
#include "../Analysis/Time.h"

namespace CCE
{
	void CCE::InputManager::StartUp()
	{
		DASSERT(Instance == nullptr, "InputManager was instantiated more than once!");
		Instance = this;

		auto startTime = Time::CurrentTick();
		initialized = true;

		auto endTime = Time::CurrentTick();
		double initDuration = Time::GetDurationInMicroSec(startTime, endTime);
		LOGC("InputManager initialized!", COLOR_BLUE);
	}

	void CCE::InputManager::ShutDown()
	{
		LOGC("Shutting down InputManager...", COLOR_BLUE);
		initialized = false;
		Instance = nullptr;
	}

	InputManager* InputManager::Instance = nullptr;

	/// <summary>
	/// Handle window input (Mouse + Keyoard).
	/// </summary>
	/// <param name="msg"></param>
	/// <param name="wParam"></param>
	/// <param name="lParam"></param>
	void InputManager::HandleWinInput(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		// ------------------- KEYBOARD -------------------
		case WM_KEYDOWN:
		{
			// handle normal key down events
			LOG("Key down");
			break;
		}
		case WM_SYSKEYDOWN:
		{
			// handle system key down events
			LOG("SysKey down");
			break;
		}
		case WM_KEYUP:
		{
			// handle normal key up events
			LOG("Key up");
			break;
		}
		case WM_SYSKEYUP:
		{
			// handle system key up events
			LOG("SysKey up");
			break;
		}
		// ------------------------------------------------
		
		// --------------------- MOUSE --------------------
		case WM_MOUSEMOVE:
		{
			// handle mouse movement

			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			printf("Moved mouse x: %i , y: %i \n", xPos, yPos);
			break;
		}
		case WM_LBUTTONDOWN:
		{
			// handle mouse button down
			LOG("Left Mouse button down");
			break;
		}
		case WM_LBUTTONUP:
		{
			// handle mouse button up
			LOG("Left Mouse button up");
			break;
		}
		case WM_RBUTTONDOWN:
		{
			// handle mouse button down
			LOG("Right Mouse button down");
			break;
		}
		case WM_RBUTTONUP:
		{
			// handle mouse button up
			LOG("Right Mouse button up");
			break;
		}
		case WM_MBUTTONDOWN:
		{
			// handle mouse button down
			LOG("Middle Mouse button down");
			break;
		}
		case WM_MBUTTONUP:
		{
			// handle mouse button up
			LOG("Middle Mouse button up");
			break;
		}
		case WM_XBUTTONDOWN:
		{
			// handle mouse button down

			UINT button = GET_XBUTTON_WPARAM(wParam);
			
			if (button == XBUTTON1)
			{
				// XBUTTON1 was clicked.
				LOG("Extra mouse button down");

			}
			else if (button == XBUTTON2)
			{
				// XBUTTON2 was clicked.
				LOG("Extra mouse button 2 down");
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
				LOG("Extra mouse button up");
			}
			else if (button == XBUTTON2)
			{
				// XBUTTON2 was released.
				LOG("Extra mouse button 2 up");
			}

			break;
		}
		// ------------------------------------------------
		}
	}
	
	/// <summary>
	/// Handle XInput (Controller).
	/// </summary>
	void InputManager::HandleXInput()
	{
		// TODO: Handle XInput Controller Input
	}
}
