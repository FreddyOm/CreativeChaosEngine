#pragma once

#define UNICODE
#define _UNICODE

#include <Windows.h>
#include <string>
#include <optional>


class CCE_Editor
{
private:
    bool editorRunning = false;
    HINSTANCE hInstance;

public:
    CCE_Editor(HINSTANCE _hInstance);
    ~CCE_Editor();
    int OpenEditor() noexcept;
    void ExitEditor() noexcept;
    bool EditorRunning() noexcept;

};