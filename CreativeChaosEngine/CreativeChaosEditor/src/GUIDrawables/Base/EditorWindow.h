#pragma once
#include "CCE/Core.h"
#include "CCE/String/String.h"
#include "IGUIDrawable.h"
#include <CCEngine.h>

using namespace CCE;

class EditorWindow : protected IGUIDrawable
{
public:

	/// <summary>
	/// Constructor of the editor window.
	/// </summary>
	/// <param name="windowName">The name of the window.</param>
	EditorWindow(String winName) : 
		windowName(winName)
	{ }

	/// <summary>
	/// The destructor of the editor window.
	/// </summary>
	~EditorWindow()
	{ }

	void UpdateDrawable(ImFont* font) override;

protected:
	String windowName = "";

};
