#pragma once
#include <vector>
#include "CCE/Core.h"
#include "CCE/String/String.h"
#include "CCE/CCEditor/CCEditor.h"
#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_dx11.h"
#include "../../imgui/imgui_impl_win32.h"
#include <atomic>

using namespace CCE;

class EditorWindow
{
public:

	/// <summary>
	/// Constructor of the editor window.
	/// </summary>
	/// <param name="windowName">The name of the window.</param>
	EditorWindow(String winName) : windowName(winName)
	{
		if (!initialized)
		{
			initialized = true;
			InitializeGUI();
		}
		
		editorWindows.push_back(this);
	}

	/// <summary>
	/// The destructor of the editor window.
	/// </summary>
	~EditorWindow()
	{
		UnInitializeGUI ();
	}

	/// <summary>
	/// Method called when updating the editor window.
	/// </summary>
	virtual void OnGui() = 0;


	static void PreGUIUpdate();
	static void PostGUIUpdate();
	static std::vector<EditorWindow*> GetEditorWindowPtrs();

	void UpdateWindow();
	void UnInitializeGUI() const;

private:
	void InitializeGUI() const;

protected:
	String windowName;

private:
	static std::vector<EditorWindow*> editorWindows;
	static std::atomic<bool> initialized;
	bool isOpen = true;
};
