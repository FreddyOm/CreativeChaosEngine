#pragma once
#include <Windows.h>
#include <string>
#include "editor/CCE_Editor.h"

// temporary entrance point -> change later

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    CCE_Editor* editor = new CCE_Editor(hInstance);

    try 
    {
        return editor->OpenEditor();
    }
    catch (std::exception e)
    {
        MessageBox(nullptr, (LPCWSTR)e.what(), L"[Standard Exception]", MB_OK | MB_ICONEXCLAMATION);
    }


    editor->ExitEditor();
    delete editor;

    return -1;
}
