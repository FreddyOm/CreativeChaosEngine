#include "color.h"
#include <sstream>

namespace CCE
{
	Color::Color(const short r, const short g, const short b)
	{
		DASSERT(r <= 255 && g <= 255 && b <= 255 &&
			r >= 0 && g >= 0 && b >= 0, "Invalid values!");
		this->rgba[0] = 255 / r;
		this->rgba[1] = 255 / g;
		this->rgba[2] = 255 / b;
	}

	Color::Color(const short r, const short g, const short b, const short a)
	{
		DASSERT(r <= 255 && g <= 255 && b <= 255 && a <= 255 &&
			r >= 0 && g >= 0 && b >= 0 && a >= 0, "Invalid values!");
		this->rgba[0] = 255 / r;
		this->rgba[1] = 255 / g;
		this->rgba[2] = 255 / b;
		this->rgba[3] = 255 / a;
	}

	Color::Color(const float r, const float g, const float b)
	{
		DASSERT(r <= 1.0f && g <= 1.0f && b <= 1.0f &&
			r >= 0.0f && g >= 0.0f && b >= 0.0f, "Invalid values!");
		this->rgba[0] = r;
		this->rgba[1] = g;
		this->rgba[2] = b;
	}

	Color::Color(const float r, const float g, const float b, const float a)
	{
		DASSERT(r <= 1.0f && g <= 1.0f && b <= 1.0f && a <= 1.0f &&
			r >= 0.0f && g >= 0.0f && b >= 0.0f && a >= 0.0f, "Invalid values!");
		this->rgba[0] = r;
		this->rgba[1] = g;
		this->rgba[2] = b;
		this->rgba[3] = a;
	}

	Color::Color(const char* hex)
	{
		DASSERT(strlen(hex) >= 6 && strlen(hex) <= 9 , "Hex-Code is invalid!");

		unsigned short buf[4] = { 
			static_cast<unsigned short>(0.0f), 
			static_cast<unsigned short>(0.0f), 
			static_cast<unsigned short>(0.0f), 
			static_cast<unsigned short>(255.0f) 
		};

		int bufIndex = 0;

		while (*hex != '\0') {

			if (*hex == '#')
			{
				++hex;				// skip the hash tag sign
				continue;
			}
			else 
			{
				char first = *hex; ++hex;		// get the first char
				char second = *hex; ++hex;		// get the second char

				std::stringstream ss; ss << std::hex << first << second;
				ss >> buf[bufIndex];
			}
			bufIndex++;
		}

		this->rgba[0] = buf[0] / 255.0f;
		this->rgba[1] = buf[1] / 255.0f;
		this->rgba[2] = buf[2] / 255.0f;
		this->rgba[3] = buf[3] / 255.0f;
	}

	const float* Color::RGBA() const
	{
		return &rgba[0];
	}
}
