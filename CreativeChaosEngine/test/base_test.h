#pragma once
#ifdef _DEBUG

#include <iostream>
#include <sstream>
#include <windows.h>
#include "../core/debug/Debug.h"
#include "../core/debug/debug_console.h"

using namespace CCE_Debug;

struct BaseTest
{
public:
	BaseTest()
	{
		if (instance == nullptr)
		{
			instance = this;
		}
	}

	~BaseTest()
	{
		
	}

	static BaseTest* instance;

	virtual void RunAllTests()
	{
		std::wostringstream oss;
		oss << L"No tests in base test class. If you see this," <<
			L" this is an error. Check BaseTest class.";
		DebugConsole::ConsoleLog(oss.str(), 14);
	}

	void PrintTestFailInfo(const wchar_t* testname, std::wstring file, int line)
	{
		std::wostringstream oss;
		oss <<  L"[FAILED]: " << testname << L" in " << file << L" at line " << line;
		DebugConsole::ConsoleLog(oss.str(), 12);
	}

	void PrintTestSuccessInfo(const wchar_t* testname)
	{
		std::wostringstream oss;
		oss << L"[SUCCEEDED]: " << testname;
		DebugConsole::ConsoleLog(oss.str(), 10);
	}
};
#endif // only test if DEBUG
