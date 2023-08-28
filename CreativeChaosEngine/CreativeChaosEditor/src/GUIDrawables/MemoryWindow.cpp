#include "MemoryWindow.h"

void MemoryWindow::OnGui()
{
	ImGui::Text("Debug Memory: %d B / %d B",
		(int)GET_EDITOR_FLOAT("debugMemUsed"), 
		(int)GET_EDITOR_FLOAT("debugMemTotal"));
	ImGui::ProgressBar(GET_EDITOR_FLOAT("debugMemUsed") / 
		GET_EDITOR_FLOAT("debugMemTotal"), ImVec2(200, 25));
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Text("Rendering Memory: %d B / %d B",
		(int)GET_EDITOR_FLOAT("rendMemUsed"),
		(int)GET_EDITOR_FLOAT("rendMemTotal"));
	ImGui::ProgressBar(GET_EDITOR_FLOAT("rendMemUsed") /
		GET_EDITOR_FLOAT("rendMemTotal"), ImVec2(200, 25));
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Text("Job Memory: %d B / %d B",
		(int)GET_EDITOR_FLOAT("jobMemUsed"),
		(int)GET_EDITOR_FLOAT("jobMemTotal"));
	ImGui::ProgressBar(GET_EDITOR_FLOAT("jobMemUsed") /
		GET_EDITOR_FLOAT("jobMemTotal"), ImVec2(200, 25));
}
