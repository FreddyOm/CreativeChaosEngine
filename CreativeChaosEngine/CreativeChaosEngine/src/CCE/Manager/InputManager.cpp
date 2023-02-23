#include "InputManager.h"
#include "../Analysis/Logger.h"
#include "../Analysis/Debug.h"
#include "../Analysis/Time.h"

#define BUTTON_STATE CCE::Input::InputDevice::ButtonState
#define MOUSE_SENS_STATE CCE::Input::InputDevice::MouseSensorState

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
			LOG_INPUT("Key down");
			break;
		}
		case WM_SYSKEYDOWN:
		{
			// handle system key down events
			LOG_INPUT("SysKey down");
			break;
		}
		case WM_KEYUP:
		{
			// handle normal key up events
			LOG_INPUT("Key up");
			break;
		}
		case WM_SYSKEYUP:
		{
			// handle system key up events
			LOG_INPUT("SysKey up");
			break;
		}
		// ------------------------------------------------
		
		// --------------------- MOUSE --------------------
		//TODO: Fix mouse delta not going to 0 when no input is detected
		case WM_MOUSEMOVE:
		{
			// handle mouse movement
			mouse.xPos = GET_X_LPARAM(lParam);
			mouse.yPos = GET_Y_LPARAM(lParam);

			DASSERT(TrackMouseEvent(lpMouseTrack), "Tracking mouse events unsuccessful!");

			if (mouse.positionState != MOUSE_SENS_STATE::JUST_MOVED &&
				mouse.positionState != MOUSE_SENS_STATE::MOVING)
			{
				mouse.positionState =
					mouse.positionState == MOUSE_SENS_STATE::JUST_MOVED ?
					MOUSE_SENS_STATE::MOVING :
					MOUSE_SENS_STATE::JUST_MOVED;

				if (mouse.positionState == MOUSE_SENS_STATE::JUST_MOVED)
				{
					// mouse just moved event
				}
			}
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
		//TODO: Fix mouse wheel not going to 0 when no input is detected
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
	}
	
	/// <summary>
	/// Handle XInput (Controller).
	/// </summary>
	void InputManager::HandleXInput()
	{
		// TODO: Handle XInput Controller Input
	}
}
