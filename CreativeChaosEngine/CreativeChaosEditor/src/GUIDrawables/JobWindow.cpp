#include "JobWindow.h"

void JobWindow::OnGui()
{
	ImGui::Text("Threadpool size: %d", GET_EDITOR_INT("threadPoolSize"));
	ImGui::Text("Fiberpool size: %d", GET_EDITOR_INT("fiberPoolSize"));
	ImGui::Text("Max. Fibers / Frame: %d", GET_EDITOR_INT("fibersPerFrame"));
	ImGui::Spacing();
	ImGui::Text("Jobs HIGH: %d", GET_EDITOR_INT("jobWaitListHigh"));
	ImGui::Text("Jobs NORMAL: %d", GET_EDITOR_INT("jobWaitListNormal"));
	ImGui::Text("Jobs LOW: %d", GET_EDITOR_INT("jobWaitListLow"));
	// TODO: Add metrics for:
	//							- Jobs per frame
	//							-  

}
