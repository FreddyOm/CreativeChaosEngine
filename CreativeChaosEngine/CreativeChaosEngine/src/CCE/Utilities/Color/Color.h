#pragma once
#include "../../Core.h"
#include "../../Analysis/Debug.h"

namespace CCE
{
	struct CCE_API Color
	{
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		float a = 1.0f;

		Color() = default;
		Color(const short r, const short g, const short b);
		Color(const short r, const short g, const short b, const short a);
		Color(const float r, const float g, const float b);
		Color(const float r, const float g, const float b, const float a);
		Color(const char* hex);
	};
}


