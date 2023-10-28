#pragma once
#include "../Core.h"
#include "../Analysis/Debug.h"
#include <string>

struct CCE_API StringConverter
{
public:
	static std::string WStringToString(const std::wstring& source)
	{
		if (source.empty()) return std::string();

		char buf[2048];
		ZeroMemory(&buf[0], sizeof(buf));
		char defaultChar = '?';
		BOOL defaultCharUsed = false;

#ifdef CCE_PLATFORM_WINDOWS
		
		// Could be non-thread-safe function!
		int convertResult = WideCharToMultiByte(CP_UTF8, 0, &source[0],
			(int)source.size(), &buf[0], sizeof(buf),
			&defaultChar, &defaultCharUsed);

		if (defaultCharUsed) { DWARNING("Some characters couldn't be converted from wstring to string!"); }

		DASSERT(convertResult > 0, "Failed to convert string '%s' to narrow string!", source.c_str());

		return std::string(buf, convertResult);

#else
#error There is no implementation of 'WStringToString' for this configuration yet
#endif
	}

	static std::wstring StringToWString(const std::string& source)
	{
		if (source.empty()) return std::wstring();

		wchar_t buf[2048];
		ZeroMemory(&buf[0], sizeof(buf));

#ifdef CCE_PLATFORM_WINDOWS

		// Could be non-thread-safe function!
		int convertResult = 
			MultiByteToWideChar(CP_UTF8, 0, &source[0],
			(int)source.size(), &buf[0], sizeof(buf));
		
		DASSERT(convertResult > 0, "Failed to convert string '%s' to wide string!", source.c_str());

		return std::wstring(buf, convertResult);
#else
#error There is no implementation of 'WStringToString' for this configuration yet
#endif
	}
};
