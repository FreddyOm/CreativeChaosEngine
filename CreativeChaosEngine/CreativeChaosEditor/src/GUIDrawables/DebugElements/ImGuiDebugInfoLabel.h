#pragma once
#include "DebugInfoLabel.h"

struct ImGuiDebugInfoLabel : public DebugInfoLabel
{
public:
	ImGuiDebugInfoLabel(CCE::String labelName,
		ImFont* pFont, float* pImGuiDrawTime, ImVec2 size = ImVec2(0, 0),
		ImVec4 bg_color = ImVec4(0.5, 0.5, 0.1, 0.2),
		ImVec4 text_color = ImVec4(1, 1, 0.2, 0.8f))
		: DebugInfoLabel(labelName, pFont, false, size, bg_color, text_color), 
		pImGuiDrawTime(pImGuiDrawTime)
	{}

	void Draw() override
	{
		if (!isActive) { return; } // If label is not activated, don't show anything

		ImGui::PushFont(pFont);
		ImGui::PushStyleColor(ImGuiCol_Text, text_color);
		ImGui::PushStyleColor(ImGuiCol_Button, bg_color);

		DebugInfoLabel::Draw();
		ImGui::DynamicTextButton("ImGui Update %.2fms", 0, size, *pImGuiDrawTime);

		ImGui::PopStyleColor(2);
		ImGui::PopFont();
	}

private:
	float* pImGuiDrawTime = nullptr;

};