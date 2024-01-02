#pragma once
#include "DebugInfoLabel.h"

struct FrameTimeDebugInfoLabel : public DebugInfoLabel
{
public:
	FrameTimeDebugInfoLabel(CCE::String labelName,
		ImFont* pFont, ImVec2 size = ImVec2(0, 0),
		ImVec4 bg_color = ImVec4(0.5f, 0.5f, 0.5f, 0.2f),
		ImVec4 text_color = ImVec4(1, 1, 1, 0.8f))
		: DebugInfoLabel(labelName, pFont, true, size, bg_color, text_color)
	{}

	void Draw() override
	{
		if (!isActive) { return; } // If label is not activated, don't show anything

		ImGui::PushFont(pFont);
		ImGui::PushStyleColor(ImGuiCol_Text, text_color);
		ImGui::PushStyleColor(ImGuiCol_Button, bg_color);

		DebugInfoLabel::Draw();
		ImGui::DynamicTextButton("%.1d (%.1fms, avg %.1fms)", 0, size,
			static_cast<int>(((1.0 / Time::deltaTime) * 1000.0)), Time::deltaTime, Time::GetAverageFrameTime());
		ImGui::SameLine();
		ImGui::TextUnformatted("|");

		ImGui::PopStyleColor(2);
		ImGui::PopFont();
	}
};
