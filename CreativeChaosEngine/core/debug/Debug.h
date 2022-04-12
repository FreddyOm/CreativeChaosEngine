#pragma once
#include <sstream>

/* ------------------------------------------------------------
	TODO: Implement custom string class instead of std::wstring
------------------------------------------------------------ */


struct Debug
{
	// Logging
	static void PrintLog(std::wstring log) noexcept;
	static void PrintLog(std::string log) noexcept;
	static void PrintLog(char* log) noexcept;

	// Error

	static void PrintError(std::wstring error) noexcept;
	static void PrintError(std::string error) noexcept;
	static void PrintError(char* error) noexcept;

	// Assert

	static void PrintAssert(bool condition, std::wstring error) noexcept;
	static void PrintAssert(bool condition, std::string error) noexcept;
	static void PrintAssert(bool condition, char* error) noexcept;
};