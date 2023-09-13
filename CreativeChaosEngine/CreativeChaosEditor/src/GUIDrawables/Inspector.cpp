#include "Inspector.h"
#include "CCE/Graphics/RenderPipeline.h"

void Inspector::OnGui()
{
	using CCE::Graphics::RenderPipeline;

	float posVals[3] = { RenderPipeline::Instance->testMesh->transform.Position().x,
						RenderPipeline::Instance->testMesh->transform.Position().y,
						RenderPipeline::Instance->testMesh->transform.Position().z };

	float rotVals[3] = { RenderPipeline::Instance->testMesh->transform.Rotation().x,
						RenderPipeline::Instance->testMesh->transform.Rotation().y,
						RenderPipeline::Instance->testMesh->transform.Rotation().z };

	float scaleVals[3] = { RenderPipeline::Instance->testMesh->transform.Scale().x,
						RenderPipeline::Instance->testMesh->transform.Scale().y,
						RenderPipeline::Instance->testMesh->transform.Scale().z };

	ImGui::Text("Transform");
	ImGui::InputFloat3("Position", posVals, "%.2f");
	ImGui::InputFloat3("Rotation", rotVals, "%.2f");
	ImGui::InputFloat3("Scale", scaleVals, "%.2f");
	ImGui::Spacing();

	RenderPipeline::Instance->testMesh->transform.SetPosition({ posVals[0], posVals[1], posVals[2] });
	RenderPipeline::Instance->testMesh->transform.SetRotation({ rotVals[0], rotVals[1], rotVals[2] });
	RenderPipeline::Instance->testMesh->transform.SetScale({ scaleVals[0], scaleVals[1], scaleVals[2]});
}
