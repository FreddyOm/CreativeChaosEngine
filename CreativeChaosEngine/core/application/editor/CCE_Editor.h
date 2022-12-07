#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include "EditorWindow.h"


class CCE_Editor
{
private:
    bool editorRunning = false;
    HINSTANCE hInstance;
    EditorWindow* editorWindow;

public:
    CCE_Editor(HINSTANCE _hInstance);
    ~CCE_Editor();
    int OpenEditor() noexcept;
    void ExitEditor() noexcept;
    bool EditorRunning() noexcept;

};