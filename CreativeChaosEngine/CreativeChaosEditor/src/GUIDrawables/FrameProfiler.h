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
    bool collectingData = false;
    int sampleCount = 0;
    ImVector<ProfilingManager::ProfilingData> items{};
};
