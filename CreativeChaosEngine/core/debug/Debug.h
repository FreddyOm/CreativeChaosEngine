#pragma once
#include <sstream>

/* ------------------------------------------------------------
	TODO: Implement custom string class instead of std::wstring
------------------------------------------------------------ */

#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
#define __WFILE__ WIDE1(__FILE__)

namespace CCE_Debug
{
#define LOG(log) (Debug::PrintLog(log))
#define CCERROR(error) (Debug::PrintError(error,__WFILE__,__LINE__))
#define ASSERT(condition,error) (Debug::PrintAssert(condition,error,__WFILE__,__LINE__))

	struct Debug
	{
		// Logging
		static void PrintLog(wchar_t* log) noexcept;

		// Error
		static void PrintError(std::wstring error, std::wstring file, unsigned int line) noexcept;

		// Assert
		static void PrintAssert(bool condition, std::wstring error, std::wstring file, unsigned int line) noexcept;

	};
}