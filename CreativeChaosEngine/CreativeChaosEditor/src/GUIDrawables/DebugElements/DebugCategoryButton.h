#pragma once
#include "../Base/IGUIDrawable.h"
#include "CCE/String/String.h"
#include "../../imgui/imgui.h"

/// <summary>
/// This is a wrapper for reused functionality. It is NOT a drawable itself,
/// but rather an aggregation of 
/// </summary>
struct DebugCategoryButton
{
public:
	DebugCategoryButton(ImVec2 size, std::vector<IGUIDrawable*>* pDrawables, CCE::String id)
	: size(size), pDrawables(pDrawables), id(id)
	{ }

	void Draw(size_t buttonIndex) const;


private:
	ImVec2 size;
	std::vector<IGUIDrawable*>* pDrawables;
	CCE::String id = "default";

	static ImVec4 normalColor;
	static ImVec4 selectedColor;
};