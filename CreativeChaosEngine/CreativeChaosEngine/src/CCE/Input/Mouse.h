#pragma once
#include "../Core.h"
#include "InputDevice.h"

namespace CCE::Input
{
	// @TODO: Keep left handed mouse layout in mind
	// @TODO: Store input values per bit in DWORD (or similar)
	struct CCE_API Mouse : public InputDevice  // 64 bytes
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
		byte padding[12] = { 0 };			// 12 bytes
	};
}
