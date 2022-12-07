#pragma once

#ifdef _DEBUG

#include "base_test.h"
#include "../core/application/editor/CCE_Editor.h"
#include "../core/application/editor/EditorWindow.h"

class CCE_Editor;
class EditorWindow;

struct EditorTest : BaseTest
{
public:
	EditorTest();
	~EditorTest();

	CCE_Editor* editor = nullptr;
	EditorWindow* window = nullptr;

	// tests
	void RunAllTests() override;

	// editor
	bool TestEditorRunning();
	bool TestEditorExit();

	// window
	bool TestWindowHandle();
	bool TestWindowClass();
	bool TestWindowClassName();
	bool TestWindowSize();
};
#endif // only test if DEBUG
