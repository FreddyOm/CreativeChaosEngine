#pragma once
#include "DebugInfoLabel.h"
#include "../Icons.h"
#include <CCEngine.h>

struct RuntimePauseLabel : public DebugInfoLabel
{
public:
	RuntimePauseLabel(CCE::String labelName,
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
		
		if (ImGui::DynamicTextButton(CCE::Application::Instance->IsPaused() ?  ICON_FK_PLAY : ICON_FK_PAUSE))
		{
			// Toggle console pause
			CCE::Application::Instance->IsPaused() ? CCE::Application::Instance->Resume() : CCE::Application::Instance->Pause();
		}

		ImGui::PopStyleColor(2);
		ImGui::PopFont();
	}
};
