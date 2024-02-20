#include "FrameProfiler.h"
#include "CCE/Analysis/Logger.h"
#include <algorithm>

void FrameProfiler::OnGui()
{
	// @TODO: Create Button for start collecti´ng and end collecting. Start Collecting resets the frame debugger 
	// and end collecting copies the CCE::ProfilingManager::Instance->m_profileData structure and loads it into a 
	// sortable table. Also add min and max exec time to indicate the variation. Make sure the profiler is only 
	// reevaluating the min/max exec time per frame and not resettingt the calls. Maybe differ between calls and 
	// calls per frame ?

	std::string rec = std::string(ICON_FK_PLAY) + std::string(" Record");
	std::string stopRec = std::string(ICON_FK_STOP) + std::string(" Stop Recording");

	if (ImGui::Button(collectingData ? stopRec.c_str() : rec.c_str(), ImVec2(120, 30)))
	{
		collectingData = !collectingData;

		if (collectingData)
		{
			// Collect data here
            items.clear();
            ProfilingManager::Instance->ResetFrameDebugger();
		}
		else
		{
            // Create item list
            if (items.Size == 0 && CCE::ProfilingManager::Instance->m_profileData.size() > 0)
            {
                items.resize(CCE::ProfilingManager::Instance->m_profileData.size(), ProfilingManager::ProfilingData());
                int n = 0;
                for (auto& kvp : CCE::ProfilingManager::Instance->m_profileData)
                {
                    auto& item = items[n++];
                    item.funcName = kvp.second.funcName;
                    item.minExecTime = kvp.second.minExecTime;
                    item.maxExecTime = kvp.second.maxExecTime;
                    item.callsPerFrame = kvp.second.callsPerFrame;
                    item.totalCalls = kvp.second.totalCalls;
                }
            }
		}
	}

    ImGui::SameLine();
    
    if (ImGui::Button("Reset", ImVec2(120, 30)))
    {
        collectingData = false;
        sampleCount = 0;
        items.clear();
    }

    if(collectingData)
        sampleCount = ProfilingManager::Instance->sampleCount;

    if (sampleCount > 0)
    {
        ImGui::Text("Samples: %d", sampleCount + 1);
        ImGui::Spacing();
    }
    static ImGuiTableFlags flags =
        ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
        | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoBordersInBody
        | ImGuiTableFlags_ScrollY;

    if (ImGui::BeginTable("Functions", 5, flags, ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 25), 0.0f))
    {
        ImGui::TableSetupColumn("Function Name", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, 0);
        ImGui::TableSetupColumn("Min Execution Time", ImGuiTableColumnFlags_PreferSortDescending | ImGuiTableColumnFlags_WidthFixed, 0.0f, 1);
        ImGui::TableSetupColumn("Max Execution Time", ImGuiTableColumnFlags_PreferSortDescending | ImGuiTableColumnFlags_WidthFixed, 0.0f, 2);
        ImGui::TableSetupColumn("Calls / Frame", ImGuiTableColumnFlags_WidthFixed, 0.0f, 3);
        ImGui::TableSetupColumn("Total Calls", ImGuiTableColumnFlags_WidthFixed, 0.0f, 4);

        ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
        ImGui::TableHeadersRow();

        // Sort our data if sort specs have been changed!
        if (ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs())
            if (sorts_specs->SpecsDirty)
            {
                if (items.Size > 1)
                    std::sort(items.begin(), items.end(), [sorts_specs](const ProfilingManager::ProfilingData& lhs, const ProfilingManager::ProfilingData& rhs) {

                    for (int n = 0; n < sorts_specs->SpecsCount; ++n)
                    {
                        // Here we identify columns using the ColumnUserID value that we ourselves passed to TableSetupColumn()
                        // We could also choose to identify columns based on their index (sort_spec->ColumnIndex), which is simpler!
                        const ImGuiTableColumnSortSpecs* sort_spec = &sorts_specs->Specs[n];
                        int delta = 0;
                        switch (sort_spec->ColumnUserID)
                        {
                        case 0:     delta = (lhs.funcName.sId - rhs.funcName.sId);   break;
                        case 1:     delta = (lhs.minExecTime - rhs.minExecTime);     break;
                        case 2:     delta = (lhs.maxExecTime - rhs.maxExecTime);     break;
                        case 3:     delta = (lhs.callsPerFrame - rhs.callsPerFrame); break;
                        case 4:     delta = (lhs.totalCalls - rhs.totalCalls);       break;
                        default: IM_ASSERT(0); break;
                        }
                        if (delta > 0)
                            return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? true : false;
                        if (delta < 0)
                            return (sort_spec->SortDirection == ImGuiSortDirection_Descending) ? true : false;
                    }

                    // qsort() is instable so always return a way to differenciate items.
                    // Your own compare function may want to avoid fallback on implicit sort specs e.g. a Name compare if it wasn't already part of the sort specs.
                    return lhs.funcName.sId > rhs.funcName.sId;
                });

                sorts_specs->SpecsDirty = false;
            }

        // Demonstrate using clipper for large vertical lists
        ImGuiListClipper clipper;
        clipper.Begin(items.Size);
        while (clipper.Step())
        for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
        {
            // Display a data item
            ProfilingManager::ProfilingData* item = &items[row_n];
            ImGui::PushID(item->funcName.sId);
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(item->funcName.Value());
            ImGui::TableNextColumn();
            ImGui::Text("%d micros", item->minExecTime);
            ImGui::TableNextColumn();
            ImGui::Text("%d micros", item->maxExecTime);
            ImGui::TableNextColumn();
            ImGui::Text("%d", item->callsPerFrame);
            ImGui::TableNextColumn();
            ImGui::Text("%d", item->totalCalls);
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
}
