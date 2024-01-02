#pragma once
#include "DebugInfoLabel.h"

struct DebugLabelFilter : public DebugInfoLabel
{
public:
	DebugLabelFilter(CCE::String labelName,
		ImFont* pFont, std::vector<DebugInfoLabel*>* pLabels, ImVec2 size = ImVec2(0, 0),
		ImVec4 bg_color = ImVec4(0.5f, 0.5f, 0.5f, 0.2f),
		ImVec4 text_color = ImVec4(1, 1, 1, 0.8f))
		: DebugInfoLabel(labelName, pFont, true, size, bg_color, text_color),
		pLabels(pLabels)
	{}

	void Draw() override
	{
		if (!isActive) { return; } // If label is not activated, don't show anything

		ImGui::PushFont(pFont);
		ImGui::PushStyleColor(ImGuiCol_Text, text_color);
		ImGui::PushStyleColor(ImGuiCol_Button, bg_color);

		DebugInfoLabel::Draw();
		if (ImGui::DynamicTextButton("+", 0, size))
			ImGui::OpenPopup("LabelFilter");
		if (ImGui::BeginPopup("LabelFilter"))
		{
			for (int i = 0; i < pLabels->size(); i++)
			{
				// User should not be able to (de-)activate the filter label itself
				if(pLabels->at(i) != this)
					ImGui::MenuItem(pLabels->at(i)->labelName.Value(), "", &pLabels->at(i)->isActive);
			}
			
			ImGui::Separator();
			
			if (ImGui::Selectable("Activate All"))
			{
				for (auto* label : *pLabels)
				{
					if(label != this)
						label->isActive = true;
				}
			}
			if (ImGui::Selectable("Deactivate All"))
			{
				for (auto* label : *pLabels)
				{
					if (label != this)
						label->isActive = false;
				}
			}

			ImGui::EndPopup();
		}

		ImGui::PopStyleColor(2);
		ImGui::PopFont();
	}

private:
	std::vector<DebugInfoLabel*>* pLabels = nullptr;

};