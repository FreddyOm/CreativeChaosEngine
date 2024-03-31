#pragma once
#include "../core.h"
#include "input-device.h"

namespace CCE::Input
{
	// @TODO: Store input values per bit in DWORD (or similar)
	struct CCE_API Keyboard : public InputDevice // 256
	{
		ButtonState keys[256] = {};
	};
}