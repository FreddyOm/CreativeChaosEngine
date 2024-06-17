#pragma once
#include "Base/EditorWindow.h"

class JobWindow : public EditorWindow
{
public:

    JobWindow(String winName) 
        : EditorWindow(winName)
    { }

    void OnGui();

    void DrawSequencer();

    void DrawThreadSequence(int& threadNum, std::pair<const DWORD, std::vector<CCE::Debug::JobProfilingData>>& thread);

private:
    bool sampling = false;

    // Frame bar histogram
    float maxFrameBarValue = 0.0001f;

    // Timeline job bars
    float baseBarWidth = 10.0f;
    float barHeight = 15.0f;

    float zoomFactor = 100.0f;
    
    std::unordered_map<DWORD, std::vector<Debug::JobProfilingData>> frameProfilingCache{};
    Time::time::time_point frameStart{};

    #define NUM_BAR_COLORS 10

    ImVec4 colors[NUM_BAR_COLORS] = {
        ImVec4(0.2, 0.2, 0.8, 1.0),
        ImVec4(0.8, 0.2, 0.2, 1.0),
        ImVec4(0.2, 0.8, 0.2, 1.0),
        ImVec4(0.2, 0.8, 0.8, 1.0),
        ImVec4(0.8, 0.8, 0.2, 1.0),
        ImVec4(0.8, 0.8, 0.8, 1.0),
        ImVec4(0.8, 0.4, 0.4, 1.0),
        ImVec4(0.8, 0.4, 0.8, 1.0),
        ImVec4(0.4, 0.4, 0.8, 1.0),
        ImVec4(0.4, 0.8, 0.8, 1.0),
    };
};
