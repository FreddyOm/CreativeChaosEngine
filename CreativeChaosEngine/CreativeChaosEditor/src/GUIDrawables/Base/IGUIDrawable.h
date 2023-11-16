#pragma once
#include <vector>
#include "CCE/Core.h"
#include "CCE/String/String.h"
#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_dx11.h"
#include "../../imgui/imgui_impl_win32.h"
#include <atomic>

using namespace CCE;

struct IGUIDrawable
{
public:
	IGUIDrawable(bool isOpen = false)
		: isOpen(isOpen)
	{
		if (!initialized)
		{
			initialized = true;
			InitializeGUI();
		}

		guiDrawables.push_back(this);
	}

	~IGUIDrawable()
	{
		UnInitializeGUI();
	}

	/// <summary>
	/// Method called when updating the editor window.
	/// </summary>
	virtual void OnGui() = 0;

	/// <summary>
	/// Method that specifies how the OnGui is called.
	/// </summary>
	virtual void UpdateDrawable() = 0;

	static void PreGUIUpdate();
	static void PostGUIUpdate();
	static std::vector<IGUIDrawable*>* GetGUIDrawablePtrs();

	void ToggleWindow();

private:
	void UnInitializeGUI() const;
	void InitializeGUI();

protected:
	bool isOpen = false;
	static std::atomic<bool> initialized;
	
	ImFont *inter_light = nullptr;
	ImFont *inter_bold = nullptr;
	ImFont *lexend_light = nullptr;

private:
	static std::vector<IGUIDrawable*> guiDrawables;
	static ImDrawData* p_drawData;
};
