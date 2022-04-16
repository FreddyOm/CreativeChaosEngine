#pragma once
#include <Windows.h>
#include <string>
#include "editor/CCE_Editor.h"

// temporary entrance point -> change later

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    CCE_Editor* editor = new CCE_Editor(1280,720, "Creative Chaos Machine - v0.1");

    try 
    {
        return editor->OpenEditor();
    }
    catch (std::exception e)
    {
        MessageBox(nullptr, e.what(), "[Standard Exception]", MB_OK | MB_ICONEXCLAMATION);
    }

    editor->~CCE_Editor();
    delete editor;

    return -1;
}
