#include "CCE_Editor.h"
#include "../../core/debug/Debug.h"

CCE_Editor::CCE_Editor()
{
    editorRunning = true;
}

CCE_Editor::~CCE_Editor()
{
    p_editorWinManager->~EditorWindowManager();
    delete p_editorWinManager;
    editorRunning = false;
}

void CCE_Editor::OpenEditor(bool open_console, WNDPROC proc) noexcept
{
    p_editorWinManager = new EditorWindowManager();
    p_editorWinManager->CreateEditorWindow(proc);

    if (open_console)
    {
        OpenConsoleWindow();
    }

    Debug::PrintLog("Test");
}

void CCE_Editor::ExitEditor() noexcept
{
    editorRunning = false;
    CCE_Editor::~CCE_Editor();
}

bool CCE_Editor::UpdateEditor() noexcept
{
    return p_editorWinManager->UpdateEditorWindows();
}

bool CCE_Editor::EditorRunning() noexcept
{
    return editorRunning;
}

void CCE_Editor::OpenConsoleWindow() noexcept
{
    AllocConsole();

    HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
    int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
    FILE* hf_out = _fdopen(hCrt, "w");
    setvbuf(hf_out, NULL, _IONBF, 1);
    *stdout = *hf_out;

    HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
    hCrt = _open_osfhandle((long)handle_in, _O_TEXT);
    FILE* hf_in = _fdopen(hCrt, "r");
    setvbuf(hf_in, NULL, _IONBF, 128);
    *stdin = *hf_in;
}
