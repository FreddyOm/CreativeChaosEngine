#pragma once

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include "../editor_window/EditorWindowManager.h"

class CCE_Editor
{
public:
    CCE_Editor();
    ~CCE_Editor();
    void OpenEditor(bool open_console, WNDPROC proc) noexcept;
    void ExitEditor() noexcept;
    bool UpdateEditor() noexcept;
    bool EditorRunning() noexcept;
private:
    void OpenConsoleWindow() noexcept;

private:
    EditorWindowManager* p_editorWinManager;
    bool editorRunning = true;
};