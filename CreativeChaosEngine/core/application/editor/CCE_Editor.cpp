#include "CCE_Editor.h"
#include "../../core/debug/Debug.h"
#include <optional>
#include "EditorWindow.h"

CCE_Editor::CCE_Editor(HINSTANCE _hInstance)
{
	editorRunning = true;
	hInstance = _hInstance;

	//Debug::PrintLog("Editor is running now!");

	
	// Do stuff
	EditorWindow win = EditorWindow(hInstance, L"Test Window");


	OpenEditor();
}

CCE_Editor::~CCE_Editor()
{
	
}

int CCE_Editor::OpenEditor() noexcept
{
	return 0;
}

void CCE_Editor::ExitEditor() noexcept
{
	editorRunning = false;
	CCE_Editor::~CCE_Editor();
}

bool CCE_Editor::EditorRunning() noexcept
{
	return editorRunning;
}
