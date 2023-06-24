#include "MemoryWindow.h"

void MemoryWindow::OnGui()
{
	ImGui::Text("Debug Memory");
	ImGui::Text("%d B", 
		(int)GET_EDITOR_FLOAT("debugMemUsed"));
	ImGui::ProgressBar(GET_EDITOR_FLOAT("debugMemUsed") / 
		GET_EDITOR_FLOAT("debugMemTotal"), ImVec2(200, 25));
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Text("Rendering Memory");
	ImGui::Text("%d B", 
		(int)GET_EDITOR_FLOAT("rendMemUsed"));
	ImGui::ProgressBar(GET_EDITOR_FLOAT("rendMemUsed") /
		GET_EDITOR_FLOAT("rendMemTotal"), ImVec2(200, 25));
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Text("Job Memory");
	ImGui::Text("%d B", 
		(int)GET_EDITOR_FLOAT("jobMemUsed"));
	ImGui::ProgressBar(GET_EDITOR_FLOAT("jobMemUsed") /
		GET_EDITOR_FLOAT("jobMemTotal"), ImVec2(200, 25));
}
