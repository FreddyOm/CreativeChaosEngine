#include "RenderingDebugger.h"
#include "CCE/Graphics/RenderPipeline.h"
#include "CCE/Analysis/Time.h"

using CCE::Graphics::RenderPipeline;

void RenderingDebugger::OnGui()
{
	ImGui::Text("FPS: %d", GET_EDITOR_INT("fps"));
	ImGui::Text("Frametime (ms): %f", GET_EDITOR_FLOAT("frameTime"));
	ImGui::Text("Avg. Frametime (ms): %f", GET_EDITOR_FLOAT("avgFrameTime"));

	ImGui::Spacing();
	ImGui::Checkbox("VSync", &RenderPipeline::Instance->GetRenderPipelineConfig()->activateVSync);
}
