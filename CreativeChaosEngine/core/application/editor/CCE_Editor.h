#pragma once

#if _DEBUG
#include "../../../test/editor_test.h"
#endif // unit testing

#include <Windows.h>
#include <string>
#include <vector>

class EditorWindow;

class CCE_Editor
{
#if _DEBUG
	friend struct EditorTest;
#endif // unit testing

private:
	bool editorRunning = false;
	HINSTANCE hInstance = {};
	EditorWindow* editorWindow = {};

public:
#if _DEBUG
	CCE_Editor();
#endif // unit testing

	CCE_Editor(HINSTANCE _hInstance);
	~CCE_Editor();
	int OpenEditor() noexcept;
	void ExitEditor() noexcept;
	bool EditorRunning() noexcept;
};
