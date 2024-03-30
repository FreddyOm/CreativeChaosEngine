#pragma once
#include "../Core.h"

namespace CCE::Input
{
	struct CCE_API InputDevice
	{
		// @TODO: Evaluate whether or not it makes sense to use union here
		enum class AxisState // 2 bytes
		{
			AXIS_RELEASED,
			AXIS_MOVED,
			AXIS_JUST_RELEASED,
			AXIS_JUST_MOVED,
		};

		enum class ButtonState // 2 bytes
		{
			RELEASED = 0,
			PRESSED = 1,
			JUST_PRESSED = 2,
			JUST_RELEASED = 3,
		};

		// @TODO: Check alignment again
		struct Axis // 8 bytes
		{
			alignas (8) float value;
			alignas (8) AxisState state;
		};

		// @TODO: Check alignment again
		struct Axis2D // 16 bytes
		{
			alignas (16) Axis x;
			alignas (16) Axis y;
		};

		enum class Keycode // 131 bytes -> 136 bytes
		{
			BACKSPACE = 0x08,
			TAB = 0x09,
			CLEAR = 0x0C,
			ENTER = 0x0D,
			SHIFT = 0x10,
			CTRL = 0x11,
			ALT = 0x12,
			PAUSE = 0x13,
			CAPS_LOCK = 0x14,
			ESCAPE = 0x1B,
			SPACE = 0x20,
			PAGE_UP = 0x21,
			PAGE_DOWN = 0x22,
			END = 0x23,
			HOME = 0x24,
			
			KEY_LEFT = 0x25,
			KEY_UP = 0x26,
			KEY_RIGHT = 0x27,
			KEY_DOWN = 0x28,

			SELECT = 0x29,
			PRINT = 0x2A,
			EXECUTE = 0x2B,
			PRINT_SCREEN = 0x2C,
			INS = 0x2D,
			DEL = 0x2E,
			HELP = 0x2F,

			NUM_0 = 0x30,
			NUM_1 = 0x31,
			NUM_2 = 0x32,
			NUM_3 = 0x33,
			NUM_4 = 0x34,
			NUM_5 = 0x35,
			NUM_6 = 0x36,
			NUM_7 = 0x37,
			NUM_8 = 0x38,
			NUM_9 = 0x39,

			KEY_A = 0x41,
			KEY_B = 0x42,
			KEY_C = 0x43,
			KEY_D = 0x44,
			KEY_E = 0x45,
			KEY_F = 0x46,
			KEY_G = 0x47,
			KEY_H = 0x48,
			KEY_I = 0x49,
			KEY_J = 0x4A,
			KEY_K = 0x4B,
			KEY_L = 0x4C,
			KEY_M = 0x4D,
			KEY_N = 0x4E,
			KEY_O = 0x4F,
			KEY_P = 0x50,
			KEY_Q = 0x51,
			KEY_R = 0x52,
			KEY_S = 0x53,
			KEY_T = 0x54,
			KEY_U = 0x55,
			KEY_V = 0x56,
			KEY_W = 0x57,
			KEY_X = 0x58,
			KEY_Y = 0x59,
			KEY_Z = 0x5A,

			LEFT_WIN = 0x5B,
			RIGHT_WIN = 0x5C,
			APPS = 0x5D,
			SLEEP = 0x5F,

			NUMPAD_0 = 0x60,
			NUMPAD_1 = 0x61,
			NUMPAD_2 = 0x62,
			NUMPAD_3 = 0x63,
			NUMPAD_4 = 0x64,
			NUMPAD_5 = 0x65,
			NUMPAD_6 = 0x66,
			NUMPAD_7 = 0x67,
			NUMPAD_8 = 0x68,
			NUMPAD_9 = 0x69,

			MULT = 0x6A,
			ADD = 0x6B,
			SEPERATOR = 0x6C,
			SUBSTRACT = 0x6D,
			DECIMAL = 0x6E,
			DIVIDE = 0x6F,

			F1 = 0x70,
			F2 = 0x71,
			F3 = 0x72,
			F4 = 0x73,
			F5 = 0x74,
			F6 = 0x75,
			F7 = 0x76,
			F8 = 0x77,
			F9 = 0x78,
			F10 = 0x79,
			F11 = 0x7A,
			F12 = 0x7B,
			F13 = 0x7C,
			F14 = 0x7D,
			F15 = 0x7E,
			F16 = 0x7F,
			F17 = 0x80,
			F18 = 0x81,
			F19 = 0x82,
			F20 = 0x83,
			F21 = 0x84,
			F22 = 0x85,
			F23 = 0x86,
			F24 = 0x87,
			
			NUM_LOCK = 0x90,
			SCROLL_LOCK = 0x91,
			LEFT_SHIFT = 0xA0,		// REDUNDANT
			RIGHT_SHIFT = 0xA1,
			LEFT_CTRL = 0xA2,
			RIGHT_CTRL = 0xA3,
			LEFT_ALT = 0xA4,
			RIGHT_ALT = 0xA5,
			BROWSER_BACK = 0xA6,
			BROWSER_FORWARD = 0xA7,

			KEY_PLUS = 0xBB,
			KEY_KOMMA = 0xBC,
			KEY_MINUS = 0xBD,
			KEY_PUNKT = 0xBE,

			GR_KL_KEY = 0xE2,
			PROCESS = 0xE5,
			ATTN = 0xF6,
			CR_SEL = 0xF7,
			EX_SEL = 0xF8,
			ER_EOF = 0xF9,
			PLAY_KEY = 0xFA,
			ZOOM_KEY = 0xFB,
			PA1_KEY = 0xFD,
			CLEAR_KEY = 0xFE
		};
	};
}
