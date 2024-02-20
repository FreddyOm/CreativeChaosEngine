#pragma once
#include "Base/EditorWindow.h"
#include "CCE/Manager/ProfilingManager.h"

struct FrameProfiler : public EditorWindow
{
    FrameProfiler(String winName)
        : EditorWindow(winName)
    { }

    void OnGui();


private:

    int SortItems(const void* begin, const void* end) const;

private:
    bool collectingData = false;
    ImGuiTableSortSpecs* sortSpecs = nullptr;
    ImVector<ProfilingManager::ProfilingData> items{};
};
