#pragma once

#include <Windows.h>
#include <string>
#include <optional>

class CCE_Editor
{
    struct AppWindow
    {
        AppWindow() noexcept;
        ~AppWindow();
    private:
        AppWindow(const AppWindow&) = delete;
        AppWindow& operator = (const AppWindow&) = delete;
        static constexpr const char* wndClassName = "Creative Chaos Machine - v0.1 [PRE_ALPHA]";
        static AppWindow wndClass;
        HINSTANCE hInst;

    public:
        static const char* GetWindowClassName() noexcept;
        static HINSTANCE GetInstance() noexcept;
    };

public:
    CCE_Editor(int width, int height, const char* name);
    ~CCE_Editor();
    int OpenEditor() noexcept;
    void ExitEditor() noexcept;
    bool EditorRunning() noexcept;

    // window init
    void CreateNewWindow();
    void SetWindowTitle(const std::string& title);

private:
    bool isInitalized = false;
    bool UpdateEditor() const;

private:
    AppWindow* appWnd;
    static std::optional<int> ProcessMessages() noexcept;
    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

private:
    bool editor_running = false;
    int window_width = 1280;
    int window_height = 720;
    const char* windowName;
    HWND hWnd = {};
};