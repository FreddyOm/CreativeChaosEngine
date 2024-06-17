#pragma once
#include "Base\IGUIDrawable.h"
#include "DebugElements\DebugCategoryButton.h"

// InfoLabels
#include "DebugElements\FrameTimeDebugInfoLabel.h"
#include "DebugElements\PhysicsTimeDebugInfoLabel.h"
#include "DebugElements\ImGuiDebugInfoLabel.h"
#include "DebugElements\DebugLabelFilter.h"
#include "DebugElements\ConsoleDebugInfoLabel.h"
#include "DebugElements\RuntimePauseLabel.h"

class RuntimeDebugger : private IGUIDrawable 
{
public:

	RuntimeDebugger(double *pImgui_process_time_ms) :
		IGUIDrawable(true)
	{
		catButtons =
		{
			DebugCategoryButton(ImVec2(40,40), GetGUIDrawablePtrs(), ICON_FK_VIDEO_CAMERA),
			DebugCategoryButton(ImVec2(40,40), GetGUIDrawablePtrs(), ICON_FK_CHECK_CIRCLE_O),
			DebugCategoryButton(ImVec2(40,40), GetGUIDrawablePtrs(), ICON_FK_KEYBOARD_O),
			DebugCategoryButton(ImVec2(40,40), GetGUIDrawablePtrs(), ICON_FK_TASKS),
		};

		// Add more individual metrics here (e.g. GPU time, vertex count, mesh count, ...)
		labels =
		{
			new RuntimePauseLabel("Runtime Pause Label", inter_bold, ImVec2(labelHeight, labelHeight)),
			new FrameTimeDebugInfoLabel("Game Update Time", inter_bold, ImVec2(130, labelHeight)),
			new ImGuiDebugInfoLabel("ImGui Update Time", inter_bold, pImgui_process_time_ms, ImVec2(115, labelHeight)),
			new PhysicsTimeDebugInfoLabel("Physics Update Time", inter_bold, ImVec2(115, labelHeight)),
			new ConsoleDebugInfoLabel("Console Info", inter_bold, GetGUIDrawablePtrs(), ImVec2(130, labelHeight)),
			
			// The filter is always the last one
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
	void UpdateDrawable(ImFont* font) override;

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
