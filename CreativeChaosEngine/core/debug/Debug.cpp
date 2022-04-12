#include "Debug.h"
#include <iostream>
#include <windows.h>

#pragma region logging

void Debug::PrintLog(std::wstring log) noexcept
{
#if _DEBUG
	std::wcout << "[LOG]: " << log << std::endl;
#endif
}

void Debug::PrintLog(std::string log) noexcept
{
#if _DEBUG
	std::cout << "[LOG]: " << log << std::endl;
#endif
}

void Debug::PrintLog(char* log) noexcept
{
#if _DEBUG
	std::cout << "[LOG]: " << log << std::endl;
#endif
}

#pragma endregion logging

#pragma region error

void Debug::PrintError(std::wstring error) noexcept
{
#if _DEBUG
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout,
		FOREGROUND_RED | FOREGROUND_INTENSITY);

	std::wcout << "[ERROR]: " << error << std::endl;
#endif
}

void Debug::PrintError(std::string error) noexcept
{
#if _DEBUG
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout,
		FOREGROUND_RED | FOREGROUND_INTENSITY);

	std::cout << "[ERROR]: " << error << std::endl;
#endif
}

void Debug::PrintError(char* error) noexcept
{
#if _DEBUG
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout,
		FOREGROUND_RED | FOREGROUND_INTENSITY);

	std::cout << "[ERROR]: " << error << std::endl;
#endif
}

#pragma endregion error

#pragma region assert

void Debug::PrintAssert(bool condition, std::wstring error) noexcept
{
#if _DEBUG
	if (!condition)
	{
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdout,
			FOREGROUND_RED | FOREGROUND_INTENSITY);

		std::wcout << "[ASSERT]: " << error << std::endl;
	}
#endif
}

void Debug::PrintAssert(bool condition, std::string error) noexcept
{
#if _DEBUG
	if (!condition)
	{
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdout,
			FOREGROUND_RED | FOREGROUND_INTENSITY);

		std::cout << "[ASSERT]: " << error << std::endl;
	}
#endif
}

void Debug::PrintAssert(bool condition, char* error) noexcept
{
#if _DEBUG
	if (!condition)
	{
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdout,
			FOREGROUND_RED | FOREGROUND_INTENSITY);

		std::cout << "[ASSERT]: " << error << std::endl;
	}
#endif
}

#pragma endregion assert