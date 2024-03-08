#include "EditorWindow.h"
#include <CCEngine.h>

// @TODO: Check if the update can be called collectively by using an event

void EditorWindow::UpdateDrawable(ImFont* font = inter_light)
{
	if (initialized)
	{
		if (isOpen) 
		{
			ImGui::PushFont(font);
			ImGui::Begin(windowName.Value(), &isOpen, ImGuiWindowFlags_NoCollapse);
			OnGui();
			ImGui::End();
			ImGui::PopFont();
		}		
	}
}
