#pragma once
#include "../Core.h"
#include "InputDevice.h"

namespace CCE::Input
{
	// @TODO: Store input values per bit in DWORD (or similar)
	struct CCE_API Controller : public InputDevice // 128 bytes
	{
		ButtonState RNorth = {};		// 4 bytes
		ButtonState REast = {};			// 4 bytes
		ButtonState RSouth = {};		// 4 bytes
		ButtonState RWest = {};			// 4 bytes

		ButtonState LNorth = {};		// 4 bytes
		ButtonState LEast = {};			// 4 bytes
		ButtonState LSouth = {};		// 4 bytes
		ButtonState LWest = {};			// 4 bytes

		ButtonState RShoulder = {};		// 4 bytes
		ButtonState LShoulder = {};		// 4 bytes
		ButtonState Option1 = {};		// 4 bytes
		ButtonState Option2 = {};		// 4 bytes

		Axis RTrigger = {};				// 8 bytes
		Axis LTrigger = {};				// 8 bytes

		Axis2D RJoypad = {};			// 16 bytes

		Axis2D LJoypad = {};			// 16 bytes

		ButtonState RJoypadButton = {};	// 4 bytes
		ButtonState LJoypadButton = {};	// 4 bytes
		byte padding[24] = { 0 };		// 24 bytes
	};
}
