#include "DebugLabels.h"
#include "CCEngine.h"

void DebugLabels::Draw() const
{
	ImGui::PushFont(pFont);

	ImGui::PushStyleColor(ImGuiCol_Text, text_color);
	ImGui::PushStyleColor(ImGuiCol_Button, bg_color);

	DrawEx();

	ImGui::PopStyleColor(2);

	ImGui::PopFont();
}

void DebugLabels::DrawEx() const
{
	ImGui::DynamicTextButton("%.1d (%.1fms, avg %.1fms)", 0, ImVec2(130, 18),
		GET_EDITOR_INT("fps"), GET_EDITOR_FLOAT("frameTime"), GET_EDITOR_FLOAT("avgFrameTime"));
	ImGui::SameLine();
	ImGui::Text("|");
	ImGui::SameLine();
	ImGui::DynamicTextButton("ImGui Update %.2fms", 0, ImVec2(115, 18), *pTime);
	ImGui::SameLine();
}
