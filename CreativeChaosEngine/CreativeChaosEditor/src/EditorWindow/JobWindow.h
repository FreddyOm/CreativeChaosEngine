#pragma once
#include "Base/EditorWindow.h"

class JobWindow : public EditorWindow
{
public:

    JobWindow(String winName) : EditorWindow(winName)
    { }

    void OnGui();
};

