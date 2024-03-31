#include "renderingDebugger.h"
#include "CCE/graphics/rendering.h"
#include "CCE/analysis/time.h"

using namespace CCE::Graphics;

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

		ImGui::Checkbox("VSync", &g_RenderPipelineConfig.VSync);
	}

	if (ImGui::CollapsingHeader("Camera"))
	{
		static int projection_idx = 0;
		if (ImGui::Combo("Projection", &projection_idx, "Perspective\0Orthographic"))
		{
			switch (projection_idx)
			{
			case 0: g_pViewportCamera->SetProjectionType(projection_idx); break;
			case 1: g_pViewportCamera->SetProjectionType(projection_idx); break;
			}
		}
		
		if(projection_idx == 0)
		    ImGui::SliderFloat("Camera FOV", &g_pViewportCamera->fovVertical, 1.0F, 179.0F, "%f.1");

		ImGui::Spacing();
		ImGui::ColorPicker4("Background Color", g_RenderPipelineConfig.backgroundColor.rgba);
	}
}
