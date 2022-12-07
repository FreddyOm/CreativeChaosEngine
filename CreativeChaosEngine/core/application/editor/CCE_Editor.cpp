#include "CCE_Editor.h"
#include "../../core/debug/Debug.h"
#include <optional>
#include "EditorWindow.h"

CCE_Editor::CCE_Editor(HINSTANCE _hInstance)
{
	Debug::PrintAssert(!editorRunning, "Editor is already running!", __FILE__, __LINE__);
	
	// setup general flags
	editorRunning = true;
	hInstance = _hInstance;

	
	// create a new window instance
	editorWindow = new EditorWindow(hInstance, L"Test Window");	
}

CCE_Editor::~CCE_Editor()
{
	free(editorWindow);
}

int CCE_Editor::OpenEditor() noexcept
{
	return editorWindow->HandleMessages();
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
