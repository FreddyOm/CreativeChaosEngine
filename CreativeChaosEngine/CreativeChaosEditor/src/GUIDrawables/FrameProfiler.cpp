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
        items.clear();
    }


    // Options
    static ImGuiTableFlags flags =
        ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
        | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoBordersInBody
        | ImGuiTableFlags_ScrollY;

    if (ImGui::BeginTable("Functions", 5, flags, ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 25), 0.0f))
    {
        // Declare columns
        // We use the "user_id" parameter of TableSetupColumn() to specify a user id that will be stored in the sort specifications.
        // This is so our sort function can identify a column given our own identifier. We could also identify them based on their index!
        // Demonstrate using a mixture of flags among available sort-related flags:
        // - ImGuiTableColumnFlags_DefaultSort
        // - ImGuiTableColumnFlags_NoSort / ImGuiTableColumnFlags_NoSortAscending / ImGuiTableColumnFlags_NoSortDescending
        // - ImGuiTableColumnFlags_PreferSortAscending / ImGuiTableColumnFlags_PreferSortDescending
        ImGui::TableSetupColumn("Function Name", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, 0);
        ImGui::TableSetupColumn("Min Execution Time", ImGuiTableColumnFlags_PreferSortDescending | ImGuiTableColumnFlags_WidthFixed, 0.0f, 1);
        ImGui::TableSetupColumn("Max Execution Time", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, 2);
        ImGui::TableSetupColumn("Calls / Frame", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, 3);
        ImGui::TableSetupColumn("Total Calls", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, 4);

        ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
        ImGui::TableHeadersRow();

        // Sort our data if sort specs have been changed!
        if (ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs())
            if (sorts_specs->SpecsDirty)
            {
                sortSpecs = sorts_specs; // Store in variable accessible by the sort function.
                if (items.Size > 1)
                    //qsort(items.begin(), items.size(), sizeof(items[0]), (_CoreCrtNonSecureSearchSortCompareFunction)SortItems);
                sortSpecs = NULL;
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

int FrameProfiler::SortItems(const void* lhs, const void* rhs) const
{
    const ProfilingManager::ProfilingData* first = (const ProfilingManager::ProfilingData*)lhs;
    const ProfilingManager::ProfilingData* second = (const ProfilingManager::ProfilingData*)rhs;

    for (int n = 0; n < sortSpecs->SpecsCount; ++n)
    {
        // Here we identify columns using the ColumnUserID value that we ourselves passed to TableSetupColumn()
        // We could also choose to identify columns based on their index (sort_spec->ColumnIndex), which is simpler!
        const ImGuiTableColumnSortSpecs* sort_spec = &sortSpecs->Specs[n];
        int delta = 0;
        switch (sort_spec->ColumnUserID)
        {
        case 0:     delta = (first->funcName.sId - second->funcName.sId);   break;
        case 1:     delta = (first->minExecTime - second->minExecTime);     break;
        case 2:     delta = (first->maxExecTime - second->maxExecTime);     break;
        case 3:     delta = (first->callsPerFrame - second->callsPerFrame); break;
        case 4:     delta = (first->totalCalls - second->totalCalls);       break;
        default: IM_ASSERT(0); break;
        }
        if (delta > 0)
            return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? +1 : -1;
        if (delta < 0)
            return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? -1 : +1;
    }

    // qsort() is instable so always return a way to differenciate items.
    // Your own compare function may want to avoid fallback on implicit sort specs e.g. a Name compare if it wasn't already part of the sort specs.
    return (first->funcName.sId - second->funcName.sId);

    return 0;
}
