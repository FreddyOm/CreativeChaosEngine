#pragma once
#include "CCE/Core.h"
#include "CCE/String/String.h"
#include "../../imgui/imgui.h"

class DebugLabels
{
public:
	DebugLabels(ImFont* pFont, ImVec4 bg_color, ImVec4 text_color, float *pTime)
		: pFont(pFont), bg_color(bg_color), text_color(text_color), pTime(pTime)
	{ }

	void Draw() const;

private:
	void DrawEx() const;

private:
	ImFont* pFont = nullptr;
	ImVec4 bg_color = {};
	ImVec4 text_color = {};
	float* pTime = nullptr;
};

