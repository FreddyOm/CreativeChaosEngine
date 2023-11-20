#include "DebugCategoryButton.h"

void DebugCategoryButton::Draw(size_t buttonIndex) const
{
	if (pDrawables->at(buttonIndex)->IsOpen())
	{
		ImGui::PushStyleColor(ImGuiCol_Text, normalColor);
		ImGui::PushStyleColor(ImGuiCol_Button, selectedColor);
	}
	else 
	{
		ImGui::PushStyleColor(ImGuiCol_Text, selectedColor);
		ImGui::PushStyleColor(ImGuiCol_Button, normalColor);
	}

	if (ImGui::Button(id.Value(), size))
	{
		pDrawables->at(buttonIndex)->ToggleWindow();
	}

	ImGui::PopStyleColor(2);
}

ImVec4 DebugCategoryButton::normalColor = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
ImVec4 DebugCategoryButton::selectedColor = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);