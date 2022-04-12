#pragma once
#include <Windows.h>
#include "editor/CCE_Editor.h"
#include "../debug/Debug.h"

// temporary entrance point -> change later

LRESULT CALLBACK WindowProcessMessages(HWND hWnd, UINT msg, WPARAM param, LPARAM lparam);

CCE_Editor* editor = new CCE_Editor();

int WINAPI WinMain(HINSTANCE currentInstance, HINSTANCE previousInstance, PSTR cmdLine, INT cmdCount)
{
    editor->OpenEditor(false, WindowProcessMessages);

    while (true)
    {
        editor->UpdateEditor();
    }

    delete editor;

    return 0;
}

LRESULT CALLBACK WindowProcessMessages(HWND hWnd, UINT msg, WPARAM param, LPARAM lparam)
{
    switch (msg)
    {
    case WM_QUIT:
        editor->ExitEditor();
        break;
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
        break;
    }
    return DefWindowProc(hWnd, msg, param, lparam);
}
