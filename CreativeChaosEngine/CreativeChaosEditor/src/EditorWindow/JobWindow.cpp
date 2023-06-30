#include "JobWindow.h"

void JobWindow::OnGui()
{
	ImGui::Text("Threadpool size: %d", GET_EDITOR_INT("threadPoolSize"));
	ImGui::Text("Fiberpool size: %d", GET_EDITOR_INT("fiberPoolSize"));
	// TODO: Add metrics for:
	//							- Jobs per frame
	//							-  

}
