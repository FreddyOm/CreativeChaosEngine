#include "EditorWindow.h"
#include <CCEngine.h>

// TODO: Check if the update can be called collectively by using an event

void EditorWindow::UpdateDrawable()
{
	if (initialized)
	{
		if (isOpen) 
		{
			ImGui::Begin(windowName.Value(), &isOpen, ImGuiWindowFlags_NoCollapse);
			OnGui();
			ImGui::End();
		}		
	}
}
