#pragma once
#include "../Core.h"

namespace CCE::Input
{
	struct CCE_API InputDevice
	{
		// TODO: Evaluate whether or not it makes sense to use union here
		enum AxisState // 4 bytes
		{
			AXIS_STILL,
			AXIS_JUST_STILL,
			AXIS_MOVED,
			AXIS_JUST_MOVED
		};

		enum ButtonState // 4 bytes
		{
			RELEASED = 0,
			JUST_RELEASED = 1,
			PRESSED = 2,
			JUST_PRESSED = 3
		};

		//TODO: Check alignment again
		union Axis // 8 bytes
		{
			float value;
			AxisState state;
		};

		// TODO: Check alignment again
		union Joypad // 16 bytes
		{
			Axis x;
			Axis y;
		};
	};
}
