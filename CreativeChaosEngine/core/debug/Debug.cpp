#include "Debug.h"
#include <iostream>
#include <sstream>
#include <windows.h>

#pragma region logging

void Debug::PrintLog(std::wstring log) noexcept
{
#if _DEBUG
	MessageBox(nullptr, log.c_str(), L"[LOG]", MB_OK | MB_ICONINFORMATION);
#endif
}

#pragma endregion logging

#pragma region error

void Debug::PrintError(std::string error, std::string file ,unsigned int line) noexcept
{
#if _DEBUG
	std::ostringstream oss;

	oss << "[ASSERT]: " << error << std::endl
		<< "[FILE]: " << file << std::endl
		<< "[LINE]: " << line;

	MessageBoxW(nullptr, reinterpret_cast<LPCWSTR>(oss.str().c_str()), L"[ERROR]", MB_OK | MB_ICONERROR);
#endif
}

#pragma endregion error

#pragma region assert

void Debug::PrintAssert(bool condition, std::string error, std::string file, unsigned int line) noexcept
{
#if _DEBUG
	if (!condition)
	{
		std::ostringstream oss;

		oss << "[ASSERT]: " << error << std::endl
			<< "[FILE]: " << file << std::endl
			<< "[LINE]: " << line;

		MessageBox(nullptr, (LPCWSTR)oss.str().c_str(), L"[ASSERT]", MB_OK | MB_ICONERROR);
	}
#endif
}

#pragma endregion assert