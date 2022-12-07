
#include "CCE_Editor.h"
#include "../../debug/Debug.h"
#include <optional>
#include "EditorWindow.h"

using namespace CCE_Debug;

#if _DEBUG
/// <summary>
/// Default constructor for testing purposes only.
/// </summary>
CCE_Editor::CCE_Editor()
{

}
#endif // unit testing

/// <summary>
/// Constructor for the editor.
/// </summary>
/// <param name="_hInstance">The handle to the application instance.</param>
CCE_Editor::CCE_Editor(HINSTANCE _hInstance)
{
	ASSERT(!editorRunning, L"Editor is already running!");

	// setup general flags
	editorRunning = true;
	hInstance = _hInstance;


	// create a new window instance
	editorWindow = new EditorWindow(hInstance, L"Test Window");
}

/// <summary>
/// The destructor of the editor.
/// </summary>
CCE_Editor::~CCE_Editor()
{
	if (EditorRunning())
	{
		editorRunning = false;
	}

	free(editorWindow);
}

/// <summary>
/// This function starts the editor functionality and returns the application status.
/// </summary>
/// <returns></returns>
int CCE_Editor::OpenEditor() noexcept
{
	return editorWindow->HandleMessages();
}

/// <summary>
/// Stops the editor and closes the window aswell as calls the destructor.
/// </summary>
void CCE_Editor::ExitEditor() noexcept
{
	editorRunning = false;
	CCE_Editor::~CCE_Editor();
}

/// <summary>
/// Returns the status of the application.
/// </summary>
/// <returns>Whether or not the editor is currently running.</returns>
bool CCE_Editor::EditorRunning() noexcept
{
	return editorRunning;
}
