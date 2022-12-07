#pragma once
#include <Windows.h>
#include <string>
#include <iostream>
#include "editor/CCE_Editor.h"

#if _DEBUG
#include "../../test/editor_test.h"
#include "../debug/debug_console.h"
#endif // _DEBUG

#define UNIT_TESTING

// temporary entrance point -> change later
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#if _DEBUG

#ifdef UNIT_TESTING
	
		using namespace CCE_Debug;

		DebugConsole console = DebugConsole();

		{
			DebugConsole::ConsoleLog(L"---------------- UNIT TESTING ----------------");

			EditorTest test = EditorTest();
			test.RunAllTests();
		}

		//DebugConsole::CloseConsole();
#endif
#endif // _DEBUG

	CCE_Editor* editor = new CCE_Editor(hInstance);

	try
	{
		return editor->OpenEditor();
	}
	catch (std::exception e)
	{
		MessageBox(nullptr, (LPCWSTR)e.what(), L"[Standard Exception]", MB_OK | MB_ICONEXCLAMATION);
	}

	editor->ExitEditor();
	delete editor;

	return -1;
}
