#include "RenderingDebugger.h"
#include "CCE/Graphics/RenderPipeline.h"
#include "CCE/Analysis/Time.h"

using CCE::Graphics::RenderPipeline;

void RenderingDebugger::OnGui()
{
	if (ImGui::CollapsingHeader("Stats"))
	{
		ImGui::Text("FPS: %d", GET_EDITOR_INT("fps"));
		ImGui::Text("Frametime (ms): %f", GET_EDITOR_FLOAT("frameTime"));
		ImGui::Text("Avg. Frametime (ms): %f", GET_EDITOR_FLOAT("avgFrameTime"));

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Checkbox("VSync", &RenderPipeline::Instance->GetRenderPipelineConfig()->VSync);
	}

	if (ImGui::CollapsingHeader("Camera"))
	{
		static int projection_idx = 0;
		if (ImGui::Combo("Projection", &projection_idx, "Perspective\0Orthographic"))
		{
			switch (projection_idx)
			{
			case 0: RenderPipeline::Instance->pViewportCamera->SetProjectionType(projection_idx); break;
			case 1: RenderPipeline::Instance->pViewportCamera->SetProjectionType(projection_idx); break;
			}
		}
		
		if(projection_idx == 0)
		    ImGui::SliderFloat("Camera FOV", &RenderPipeline::Instance->pViewportCamera->fovVertical, 1.0F, 179.0F, "%f.1");

		ImGui::Spacing();
		ImGui::ColorPicker4("Background Color", RenderPipeline::Instance->GetRenderPipelineConfig()->backgroundColor.rgba);

	}
}
