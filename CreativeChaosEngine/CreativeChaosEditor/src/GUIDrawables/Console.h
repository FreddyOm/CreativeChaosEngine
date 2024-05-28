#pragma once
#include "Base/EditorWindow.h"
#include "../src/CCE/Analysis/logger.h"
#include "CCE/analysis/debugInfoDesc.h"
#include <vector>

class Console : public EditorWindow
{
public:
	Console(String _winName)
		: EditorWindow(_winName), pLogBuffer(&g_logBuffer)
	{ }

	void OnGui();

private:
	void OpenIDE(const std::string fileName, const int line);

private:
	std::vector<CCE::DebugInfoDesc>* pLogBuffer = nullptr;

	bool messages = true;
	bool warnings = true;
	bool errors = true;

	bool selectedLog = false;
	ImVec4 logColor = ImVec4(1,1,1,1);
};
