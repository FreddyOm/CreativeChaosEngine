#include "DebugCategoryButton.h"

void DebugCategoryButton::Draw(size_t buttonIndex) const
{
	if (ImGui::Button(id.Value(), size))
	{
		pDrawables->at(buttonIndex)->ToggleWindow();
	}
}
