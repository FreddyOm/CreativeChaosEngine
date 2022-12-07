#include "Debug.h"
#include <iostream>
#include <sstream>
#include <windows.h>

namespace CCE_Debug
{
#pragma region logging

	void Debug::PrintLog(wchar_t* log) noexcept
	{
#if _DEBUG
		std::wostringstream oss;
		oss << L"[LOG]: " << log;
		MessageBox(nullptr, log, L"[LOG]", MB_OK | MB_ICONINFORMATION);
#endif
	}

#pragma endregion logging

#pragma region error

	void Debug::PrintError(std::wstring error, std::wstring file, unsigned int line) noexcept
	{
#if _DEBUG
		std::wostringstream oss;

		oss << "[ERROR]: " << error << std::endl
			<< "[FILE]: " << file << std::endl
			<< "[LINE]: " << line;

		MessageBoxW(nullptr, reinterpret_cast<LPCWSTR>(oss.str().c_str()), L"[ERROR]", MB_OK | MB_ICONERROR);
#endif
	}

#pragma endregion error

#pragma region assert

	void Debug::PrintAssert(bool condition, std::wstring error, std::wstring file, unsigned int line) noexcept
	{
#if _DEBUG
		if (!condition)
		{
			std::wostringstream oss;

			oss << "[ASSERT]: " << error << std::endl
				<< "[FILE]: " << file << std::endl
				<< "[LINE]: " << line;

			MessageBox(nullptr, (LPCWSTR)oss.str().c_str(), L"[ASSERT]", MB_OK | MB_ICONERROR);
		}
#endif
	}

#pragma endregion assert
}

