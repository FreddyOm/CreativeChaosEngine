#pragma once
#include "Base\IGUIDrawable.h"
#include "DebugElements\DebugCategoryButton.h"
#include "DebugElements\DebugLabels.h"

class RuntimeDebugger : private IGUIDrawable 
{
public:

	RuntimeDebugger(float *pImgui_process_time_ms) :
		IGUIDrawable(true),
		dtLabel(inter_bold, ImVec4(0.8, 0.8, 0.8, 0.4), ImVec4(1, 1, 1, 0.8f), pImgui_process_time_ms)
	{
		catButtons =
		{
			DebugCategoryButton(ImVec2(40,40), GetGUIDrawablePtrs(), "Rendering"),
			DebugCategoryButton(ImVec2(40,40), GetGUIDrawablePtrs(), "Log"),
			DebugCategoryButton(ImVec2(40,40), GetGUIDrawablePtrs(), "Input"),
		};
	}

public:
	void UpdateDrawable() override;
	void OnGui();

private:
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize 
		| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove;

	std::vector<DebugCategoryButton> catButtons = { };
	DebugLabels dtLabel;

	ImTextureID icon_error;
	ImTextureID icon_input;
};
