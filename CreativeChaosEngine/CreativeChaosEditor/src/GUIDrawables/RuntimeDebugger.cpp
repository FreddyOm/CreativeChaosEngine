#include "RuntimeDebugger.h"
#include "CCE/Graphics/RenderPipeline.h"
#include "CCE\CCEditor\CCEditor.h"
#include "Base\EditorWindow.h"

using CCE::Graphics::RenderPipeline;

void RuntimeDebugger::UpdateDrawable()
{
	if (initialized)
	{
		OnGui();
	}
}

// TODO: Put labels into list and add bitmask in order to dynamically (de-)activate labels / buttons
// -> Use ImGui Popup

void RuntimeDebugger::OnGui()
{	
	if (isOpen)
	{
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

		ImGui::SetNextWindowPos(ImVec2(0,0));
		ImGui::SetNextWindowSize(ImVec2(80, 200));

		// Left column
		ImGui::Begin("Debug Categories", &isOpen, flags);

		for (size_t i = 0; i < catButtons.size(); ++i)
		{
			catButtons.at(i).Draw(i+1); // draw all added debug windows except this (which is at index 0)
		}

		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(50, 0));
		ImGui::SetNextWindowSize(ImVec2(460, 40));

		// Upper Row
		ImGui::Begin("FPSStats", &isOpen, flags);

		dtLabel.Draw();

		ImGui::End();
		
		ImGui::PopStyleColor(3);
	}
}
