#pragma once
#include "../Core.h"
#include "../Multithreading/job-system.h"
#include "IInputHandler.h"

namespace CCE::Input
{
	// -------------------- EXTERNAL -----------------------------

#define REGISTER_INPUT_CALLBACK Input::RegisterInputCallback(this)

	// Runtime Debugger GUI
	typedef LRESULT(*GuiInputCallback)(HWND, UINT, WPARAM, LPARAM);
	CCE_API extern GuiInputCallback InputCallback;

	CCE_API Jobs::JobReturnType Initialize();
	CCE_API Jobs::JobReturnType Deinitialize();

	CCE_API Jobs::JobReturnType RegisterInputCallback(IInputHandler* handler);
	CCE_API Jobs::JobReturnType UnregisterInputCallback(IInputHandler* handler);

	CCE_API Jobs::JobReturnType FinalizeWinInput();
	CCE_API Jobs::JobReturnType ResetInputValues();
	CCE_API Jobs::JobReturnType HandleWinInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	CCE_API Jobs::JobReturnType HandleXInput();


	// -----------------------------------------------------------


	// -------------------- INTERNAL -----------------------------

	Jobs::JobReturnType InitializeDualSense();
	Jobs::JobReturnType HandleDualSenseInput();
	Jobs::JobReturnType UpdateXInputControllerCount();
	Jobs::JobReturnType GetXInput(const DWORD controllerIndex);

	// -----------------------------------------------------------

}