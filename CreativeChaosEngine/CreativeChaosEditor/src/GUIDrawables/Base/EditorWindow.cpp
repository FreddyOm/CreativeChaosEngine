#include "EditorWindow.h"
#include <CCEngine.h>

// TODO: Check if the update can be called collectively by using an event or smth

void EditorWindow::UpdateDrawable()
{
	if (initialized)
	{
		isOpen = ImGui::BeginChild(windowName.Value());
		OnGui();
		ImGui::EndChild();
	}
}
