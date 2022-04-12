#pragma once
#include <map>
#include <sstream>
#include "EditorWindow.h"

class EditorWindowManager
{
public:
	EditorWindowManager();
	~EditorWindowManager();

	void CreateEditorWindow(WNDPROC proc);
	void CreateEditorWindow(unsigned int width, unsigned int height, WNDPROC proc);
	void CreateEditorWindow(unsigned int width, unsigned int height, std::wstring windowName, WNDPROC proc);

	void DestroyEditorWindow(unsigned short id);
	void DestroyEditorWindow(std::wstring windowName);

	bool UpdateEditorWindows();

private:
	bool MainWindowInitialized();

private:
	unsigned int DEFAULT_WIDTH = 500;
	unsigned int DEFAULT_HEIGHT = 300;
	std::wstring DEFAULT_WIN_NAME = L"New Editor Window";

private:
	unsigned short idCounter;
	std::map<unsigned short, EditorWindow> editorWindows;
	std::map<unsigned short, EditorWindow>::iterator it;
};