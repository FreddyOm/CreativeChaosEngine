#pragma once
#include "DebugInfoLabel.h"
#include "../Icons.h"

struct ConsoleDebugInfoLabel : public DebugInfoLabel
{
public:
	ConsoleDebugInfoLabel(CCE::String labelName,
		ImFont* pFont, std::vector<IGUIDrawable*>* pDrawables, ImVec2 size = ImVec2(0, 0),
		ImVec4 bg_color = ImVec4(0.5, 0.5, 0.1, 0.2),
		ImVec4 text_color = ImVec4(1, 1, 0.2, 0.8f))
		: DebugInfoLabel(labelName, pFont, false, size, bg_color, text_color), 
		pDrawables(pDrawables)
	    {}

	void Draw() override
	{
		if (!isActive) { return; } // If label is not activated, don't show anything

		AdaptColor();

		ImGui::PushFont(pFont);
		ImGui::PushStyleColor(ImGuiCol_Text, text_color);
		ImGui::PushStyleColor(ImGuiCol_Button, bg_color);

		DebugInfoLabel::Draw();
		if (ImGui::DynamicTextButton(ICON_FK_INFO_CIRCLE " %d      " ICON_FK_EXCLAMATION_CIRCLE " %d      "
			ICON_FK_BAN " %d", 0, size, Logger::logCount[0], Logger::logCount[1], Logger::logCount[2]))
		{
			// Toggle console
			pDrawables->at(2)->ToggleWindow(); // FIX-ME: Don't look for the console via an index! 
		}

		ImGui::PopStyleColor(2);
		ImGui::PopFont();
	}

	void AdaptColor()
	{
		if (Logger::logCount[2] > 0)
		{
			bg_color = ImVec4(0.5, 0.1, 0.1, 0.2);
			text_color = ImVec4(1, 0.2, 0.2, 0.8f);
			return;
		}

		if (Logger::logCount[1] > 0)
		{
			bg_color = ImVec4(0.5, 0.5, 0.1, 0.2);
			text_color = ImVec4(1, 1, 0.2, 0.8f);
			return;
		}

		bg_color = ImVec4(0.5, 0.5, 0.5, 0.2);
		text_color = ImVec4(1, 1, 1, 0.8f);
	}

private:

	std::vector<IGUIDrawable*>* pDrawables = nullptr;
};