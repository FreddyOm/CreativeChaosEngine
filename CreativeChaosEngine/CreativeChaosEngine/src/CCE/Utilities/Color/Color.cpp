#include "Color.h"
#include <sstream>

namespace CCE
{
	Color::Color(const short r, const short g, const short b)
	{
		DASSERT(r <= 255 && g <= 255 && b <= 255 &&
			r >= 0 && g >= 0 && b >= 0, "Invalid values!");
		this->r = 255 / r;
		this->g = 255 / g;
		this->b = 255 / b;
	}

	Color::Color(const short r, const short g, const short b, const short a)
	{
		DASSERT(r <= 255 && g <= 255 && b <= 255 && a <= 255 &&
			r >= 0 && g >= 0 && b >= 0 && a >= 0, "Invalid values!");
		this->r = 255 / r;
		this->g = 255 / g;
		this->b = 255 / b;
		this->a = 255 / a;
	}

	Color::Color(const float r, const float g, const float b)
	{
		DASSERT(r <= 1.0f && g <= 1.0f && b <= 1.0f &&
			r >= 0.0f && g >= 0.0f && b >= 0.0f, "Invalid values!");
		this->r = r;
		this->g = g;
		this->b = b;
	}

	Color::Color(const float r, const float g, const float b, const float a)
	{
		DASSERT(r <= 1.0f && g <= 1.0f && b <= 1.0f && a <= 1.0f &&
			r >= 0.0f && g >= 0.0f && b >= 0.0f && a >= 0.0f, "Invalid values!");
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	Color::Color(const char* hex)
	{
		DASSERT(strlen(hex) >= 6 && strlen(hex) <= 9 , "Hex-Code is invalid!");

		unsigned short buf[4] = { 0.0f, 0.0f, 0.0f, 255.0f };
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

		this->r = buf[0] / 255.0f;
		this->g = buf[1] / 255.0f;
		this->b = buf[2] / 255.0f;
		this->a = buf[3] / 255.0f;
	}
}
