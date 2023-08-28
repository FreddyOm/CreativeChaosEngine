#pragma once
#include "Base/IGUIDrawable.h"

class MenuBar : public IGUIDrawable
{
public:
	MenuBar() : IGUIDrawable()
	{ }

	void UpdateDrawable() override;

	void OnGui() override;

};

