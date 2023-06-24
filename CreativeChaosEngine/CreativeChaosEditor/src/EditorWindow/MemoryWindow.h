#pragma once
#include "Base/EditorWindow.h"


class MemoryWindow :
    public EditorWindow
{
public:

    MemoryWindow(String winName) : EditorWindow(winName)
    { }

    void OnGui();

};
