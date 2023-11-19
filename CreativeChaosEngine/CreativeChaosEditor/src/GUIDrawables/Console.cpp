#include "Console.h"
#include "CCE\Core.h"

void Console::OnGui()
{
	ImGui::PushFont(inter_bold);

	if (ImGui::Button("Clear", ImVec2(40, 20)))
	{
		Logger::ClearDebugBuffer();
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
		LogLevel type = pLogBuffer->at(i).debugType;
		
		if (type != LogLevel::NONE) 
		{ logColor = type == LogLevel::WARNING ? ImVec4(0.8, 0.8, 0, 1) : ImVec4(0.8, 0, 0, 1); }
		else { logColor = ImVec4(1, 1, 1, 1); }
			
		ImGui::PushStyleColor(ImGuiCol_Text, logColor);

		if (type == LogLevel::NONE && messages || 
			type == LogLevel::WARNING && warnings || 
			type == LogLevel::ERR && errors)
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

	ImGui::EndChild();

	ImGui::PopFont();
}

void Console::OpenIDE(const std::string fileName, const int line)
{
	ShellExecuteA(NULL,"open", fileName.c_str(), NULL, NULL, SW_SHOWNORMAL);
}
