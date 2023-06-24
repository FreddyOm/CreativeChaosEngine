#pragma once
#include "Base/EditorWindow.h"

class RenderingDebugger : public EditorWindow
{
public:
	RenderingDebugger(String _winName) :
		EditorWindow(_winName)
	{ }

	void OnGui();
};

