#include "RenderingDebugger.h"
#include "CCE/Graphics/RenderPipeline.h"
#include "CCE/Analysis/Time.h"

using CCE::Graphics::RenderPipeline;

void RenderingDebugger::OnGui()
{
	if (ImGui::CollapsingHeader("Stats"))
	{
		ImGui::Text("FPS: %d", static_cast<int>((1.0 / Time::deltaTime)));
		ImGui::Text("Frametime (ms): %f", Time::deltaTime * 1000.0);
		ImGui::Text("Avg. Frametime (ms): %f", Time::GetAverageFrameTime() * 1000.0);

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
