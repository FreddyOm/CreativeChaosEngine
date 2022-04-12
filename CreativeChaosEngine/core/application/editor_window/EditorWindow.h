#pragma once
#include <Windows.h>
#include <sstream>

class EditorWindow
{
public:
	EditorWindow(unsigned int width, unsigned int height, std::wstring winName, unsigned short windowID, WNDPROC proc);
	EditorWindow& operator=(const EditorWindow& editorWin) = delete;
	~EditorWindow();

public:
	void SetEditorWindowName(std::wstring name);
	LPCWSTR* GetEditorWindowName();
	void SetEditorWindowWidth(unsigned int width);
	void SetEditorWindowHeight(unsigned int height);
	unsigned int* GetEditorWindowWidth();
	unsigned int* GetEditorWindowHeight();

	bool UpdateWindow();

private:
	unsigned int m_width = 500;
	unsigned int m_height = 300;
	RECT m_windowRect;
	unsigned short m_windowID;
	LPCWSTR m_editorWindowName = L"New Editor Window";

private:
	LPCWSTR m_windowClassName = L"DefaultClassName";
	HINSTANCE m_hinst = {};
	HWND m_hWnd;
	WNDCLASS m_windowClass;
	MSG msg{};
};
