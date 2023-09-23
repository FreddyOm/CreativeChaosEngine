#pragma once
#include "../Core.h"

namespace CCE::Input
{
	struct Mouse;
	struct Keyboard;
	struct Controller;
	
	struct CCE_API IInputHandler
	{
	public:
		virtual void InputCallback(const Mouse* mouse, 
			const Keyboard* keyboard,
			const Controller* controller) = 0;
	};
}
