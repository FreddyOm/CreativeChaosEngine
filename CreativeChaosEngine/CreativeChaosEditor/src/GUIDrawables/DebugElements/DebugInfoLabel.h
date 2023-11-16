#pragma once
#include "CCE/Core.h"
#include "CCE/String/String.h"
#include "../../imgui/imgui.h"

struct DebugInfoLabel
{
public:
	DebugInfoLabel(CCE::String labelName, ImFont* pFont, bool isActive, 
		ImVec2 size = ImVec2(0,0),
		ImVec4 bg_color = ImVec4(0.5, 0.5, 0.5, 0.2), 
		ImVec4 text_color = ImVec4(1, 1, 1, 0.8f))
	: isActive(isActive), labelName(labelName), pFont(pFont), size(size), bg_color(bg_color), text_color(text_color)
	{ }

	virtual void Draw()
	{
		ImGui::SameLine();
	}

public:
	bool isActive = false;
	CCE::String labelName = "";

protected:
	ImFont* pFont = nullptr;
	ImVec2 size = {};
	ImVec4 bg_color = {};
	ImVec4 text_color = {};
};