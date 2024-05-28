#include "runtimeDebugger.h"
#include "CCE/graphics/rendering/D3D11/d3d11RenderPipeline.h"

void RuntimeDebugger::UpdateDrawable(ImFont* font)
{
	if (initialized)
	{
		OnGui();
	}
}

// @TODO: Put labels into list and add bitmask in order to dynamically (de-)activate labels / buttons
// -> Use ImGui Popup

void RuntimeDebugger::OnGui()
{	
	if (isOpen)
	{
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

		DrawCategoryButtons();
		DrawDebugInfoLabels();

		ImGui::PopStyleColor(3);
	}
}

void RuntimeDebugger::DrawCategoryButtons()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(80, 200));

	ImGui::Begin("Debug Categories", &isOpen, flags);

	for (size_t i = 0; i < catButtons.size(); ++i)
	{
		catButtons.at(i).Draw(i + 1); // draw all added debug windows except this (which is at index 0)
	}

	ImGui::End();
}

void RuntimeDebugger::DrawDebugInfoLabels()
{
	ImGui::SetNextWindowPos(ImVec2(50, 0));
	ImGui::SetNextWindowSize(ImVec2(600, 40));

	ImGui::Begin("FPSStats", &isOpen, flags);

	for (size_t i = 0; i < labels.size(); ++i)
	{
		labels.at(i)->Draw();
	}

	ImGui::End();
}
