#pragma once
#include "../../Core.h"
#include "../../Analysis/Debug.h"

namespace CCE
{
	struct CCE_API Color
	{
		float rgba[4];

		Color() = default;
		Color(const short r, const short g, const short b);
		Color(const short r, const short g, const short b, const short a);
		Color(const float r, const float g, const float b);
		Color(const float r, const float g, const float b, const float a);
		Color(const char* hex);

		const float* RGBA() const;

	public:

		static Color red()
		{
			return Color(1.0f, 0.0f, 0.0f);
		}

		static Color green()
		{
			return Color(0.0f, 1.0f, 0.0f);
		}

		static Color blue()
		{
			return Color(0.0f, 0.0f, 1.0f);
		}

		static Color black()
		{
			return Color(0.0f, 0.0f, 0.0f);
		}

		static Color white()
		{
			return Color(1.0f, 1.0f, 1.0f);
		}

		static Color grey()
		{
			return Color(0.5f, 0.5f, 0.5f);
		}

		static Color magenta()
		{
			return Color("#E80C7A");
		}

		static Color pink()
		{
			return Color("#FF0DFF");
		}

		static Color orange()
		{
			return Color("#FF540D");
		}

		static Color yellow()
		{
			return Color("#F2CE16");
		}

		static Color brown()
		{
			return Color("#3B290D");
		}

		static Color azure()
		{
			return Color("#4C99E5");
		}
	};
}
