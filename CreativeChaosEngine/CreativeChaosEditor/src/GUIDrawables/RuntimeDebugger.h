#pragma once
#include "Base\IGUIDrawable.h"
#include "DebugElements\DebugCategoryButton.h"

// InfoLabels
#include "DebugElements\FrameTimeDebugInfoLabel.h"
#include "DebugElements\ImGuiDebugInfoLabel.h"
#include "DebugElements\DebugLabelFilter.h"

class RuntimeDebugger : private IGUIDrawable 
{
public:

	RuntimeDebugger(float *pImgui_process_time_ms) :
		IGUIDrawable(true)
	{
		catButtons =
		{
			DebugCategoryButton(ImVec2(40,40), GetGUIDrawablePtrs(), "Rendering"),
			DebugCategoryButton(ImVec2(40,40), GetGUIDrawablePtrs(), "Log"),
			DebugCategoryButton(ImVec2(40,40), GetGUIDrawablePtrs(), "Input"),
		};

		// Add more individual metrics here (e.g. GPU time, vertex count, mesh count, ...)
		labels =
		{
			new FrameTimeDebugInfoLabel("Game Update Time", inter_bold, ImVec2(130, labelHeight)),
			new ImGuiDebugInfoLabel("ImGui Update Time", inter_bold, pImgui_process_time_ms, ImVec2(115, labelHeight)),
			new DebugLabelFilter("Filter", inter_bold, &labels, ImVec2(20, labelHeight)),
		};
	}

	~RuntimeDebugger()
	{
		for (auto* label : labels)
		{
			delete label;
		}
	}

public:
	void UpdateDrawable() override;

private:
	void OnGui();
	void DrawCategoryButtons();
	void DrawDebugInfoLabels();

private:
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize 
		| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove;

	std::vector<DebugCategoryButton> catButtons = { };
	std::vector<DebugInfoLabel*> labels = { };

	float labelHeight = 18.f;

	ImTextureID icon_error;
	ImTextureID icon_input;
};
