#pragma once
#include "Base/EditorWindow.h"

struct InputWindow : public EditorWindow
{
    InputWindow(String winName)
        : EditorWindow(winName)
    { }

    void OnGui();
};

