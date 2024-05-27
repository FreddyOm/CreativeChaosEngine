#include "Console.h"
#include "CCE\Core.h"

void Console::OnGui()
{
	if (ImGui::Button("Clear", ImVec2(40, 20)))
	{
		ClearDebugBuffer();
	}

	ImGui::SameLine();
	
	if (ImGui::Button("Filter", ImVec2(50, 20)))
		ImGui::OpenPopup("LogFilter");
	if (ImGui::BeginPopup("LogFilter"))
	{
		ImGui::MenuItem("Messages", "", &messages);
		ImGui::MenuItem("Warnings", "", &warnings);
		ImGui::MenuItem("Errors", "", &errors);

		ImGui::EndPopup();
	}

	ImGui::Separator();

	ImGui::BeginChild("Messages");

	for (size_t i = 0; i < pLogBuffer->size(); ++i)
	{
		CCE::LogLevel type = pLogBuffer->at(i).debugType;
		
		if (type != CCE::LogLevel::NONE)
		{ logColor = type == CCE::LogLevel::WARNING ? ImVec4(0.8f, 0.8f, 0, 1) : ImVec4(0.8f, 0, 0, 1); }
		else { logColor = ImVec4(1, 1, 1, 1); }
			
		ImGui::PushStyleColor(ImGuiCol_Text, logColor);

		if (type == CCE::LogLevel::NONE && messages ||
			type == CCE::LogLevel::WARNING && warnings ||
			type == CCE::LogLevel::ERR && errors)
		{
			ImGui::Selectable(pLogBuffer->at(i).msg.c_str(), &selectedLog, ImGuiSelectableFlags_AllowDoubleClick);
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			{
				// Open VS at the given position
				OpenIDE(pLogBuffer->at(i).fileName, pLogBuffer->at(i).line);
			}
			ImGui::Separator();
		}
		selectedLog = false;
		ImGui::PopStyleColor();
	}
	ImGui::SetScrollHereY(1);
	ImGui::EndChild();
}

void Console::OpenIDE(const std::string fileName, const int line)
{
	ShellExecuteA(NULL,"open", fileName.c_str(), NULL, NULL, SW_SHOWNORMAL);
}
