#pragma once
#include <sstream>

/* ------------------------------------------------------------
	TODO: Implement custom string class instead of std::wstring
------------------------------------------------------------ */

#define LOG(log) (Debug::PrintLog(log))
#define CCERROR(error) (Debug::PrintError(error,__FILE__,__LINE__))
#define ASSERT(condition,error) (Debug::PrintAssert(condition,error,__FILE__,__LINE__))

struct Debug
{
	// Logging
	static void PrintLog(char* log) noexcept;

	// Error
	static void PrintError(std::string error, std::string file, unsigned int line) noexcept;

	// Assert
	static void PrintAssert(bool condition, char* error, std::string file, unsigned int line) noexcept;

};