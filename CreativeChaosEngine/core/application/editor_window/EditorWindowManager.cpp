#include "EditorWindowManager.h"
#include "../../core/debug/Debug.h"

EditorWindowManager::EditorWindowManager()
{
	idCounter = 0;
	Debug::PrintLog("Editor Window Manager initialized...");
}

EditorWindowManager::~EditorWindowManager()
{
	for (auto window : editorWindows)
	{
		window.second.~EditorWindow();
	}

	editorWindows.clear();
	idCounter = 0;
}

/// <summary>
/// Checks if any window is active
/// </summary>
/// <returns>True if at least one window is active; False if no window is active</returns>
bool EditorWindowManager::MainWindowInitialized()
{
	return !editorWindows.empty();
}

/// <summary>
/// Creates a default editor window
/// </summary>
void EditorWindowManager::CreateEditorWindow(WNDPROC proc)
{
	CreateEditorWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, proc);
}

/// <summary>
/// Creates a default window with a specific height and width
/// </summary>
/// <param name="width">The width of the created window</param>
/// <param name="height">The height of the created window</param>
void EditorWindowManager::CreateEditorWindow(unsigned int width, unsigned int height, WNDPROC proc)
{
	CreateEditorWindow(width, height, DEFAULT_WIN_NAME, proc);
}

/// <summary>
/// Creates a window with a specific resolution and a window title
/// </summary>
/// <param name="width">The width of the created window</param>
/// <param name="height">The height of the created window</param>
/// <param name="windowName">The title of the window</param>
void EditorWindowManager::CreateEditorWindow(unsigned int width, unsigned int height, std::wstring windowName, WNDPROC proc)
{
	EditorWindow window = EditorWindow(width, height, windowName, idCounter, proc);
	editorWindows.insert(std::make_pair(idCounter, window));
	idCounter++;
}

/// <summary>
/// Destroys the editor window with the unique window id
/// </summary>
/// <param name="id">Unique id of the window to be destroyed</param>
void EditorWindowManager::DestroyEditorWindow(unsigned short id)
{
	editorWindows.erase(id);
}

/// <summary>
/// Destroys all windows with the name 'windowName'
/// </summary>
/// <param name="windowName">The name of the window(s) to be destroyed</param>
void EditorWindowManager::DestroyEditorWindow(std::wstring windowName)
{
	for (it = editorWindows.begin(); it != editorWindows.end(); it++)
	{
		if (*it->second.GetEditorWindowName() == windowName.c_str())
		{
			DestroyEditorWindow(it->first);
		}
	}
}

/// <summary>
/// Updates all editor windows in the list
/// </summary>
bool EditorWindowManager::UpdateEditorWindows()
{
	Debug::PrintAssert(MainWindowInitialized(), "Main window not initialized!");

	if (!MainWindowInitialized()) { return false; }

	for (it = editorWindows.begin(); it != editorWindows.end(); it++)
	{
		Debug::PrintAssert(it->second.UpdateWindow(), "Couldn't update window!");
	}

	//TODO: Return value according to occuring errors
	return true;
}
