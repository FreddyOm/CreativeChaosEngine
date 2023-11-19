#pragma once
#include "Base/EditorWindow.h"
#include <vector>
#include "../src/CCE/Analysis/Logger.h"

class Console : public EditorWindow
{
public:
	Console(String _winName)
		: EditorWindow(_winName), pLogBuffer(&Logger::logBuffer)
	{ }

	void OnGui();

private:
	void OpenIDE(const std::string fileName, const int line);

private:
	std::vector<Logger::DebugInfoDesc>* pLogBuffer = nullptr;

	bool messages = true;
	bool warnings = true;
	bool errors = true;

	bool selectedLog = false;
	ImVec4 logColor = ImVec4(1,1,1,1);
};
