#pragma once
#include "Base/EditorWindow.h"

class Inspector : public EditorWindow
{
public:

    Inspector(String winName)
        : EditorWindow(winName)
    { }

    void OnGui();
};

