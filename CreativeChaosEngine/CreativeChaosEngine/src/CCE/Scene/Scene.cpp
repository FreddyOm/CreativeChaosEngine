#include "Scene.h"
#include "../Manager/Application.h"
#include "../Graphics/RenderPipeline.h"
#include "../Utilities/Math/CCMath.h"

namespace CCE::Scene
{
	void Scene::SetupScene()
	{
		using namespace ECS::Components;
		using namespace Graphics;
		int y = 2;
		float bounciness = 0.0f;

		for (int x = 0; x < 3; ++x)
		{
			for (int z = 0; z < 3; ++z)
			{
				ECS::Entity entity = AddEntity();
				RenderPipeline::Instance->RenderingSystem.RegisterEntity(static_cast<long long>(entity.Id));
				Application::Instance->mPhysicsSystem.RegisterEntity(static_cast<long long>(entity.Id));

				auto& rigidbody = entity.AddComponent<Rigidbody>();
				auto& transform = entity.AddComponent<Transform>();
				auto& mesh = entity.AddComponent<Mesh>();
				auto& material = entity.AddComponent<Material>();
				auto& collider = entity.AddComponent<SphereCollider>();

				rigidbody.bounciness = bounciness;

				transform.SetTranslation({(float)x * 2.0f , (float)y, (float)z * 2.0f });
				transform.SetScale({ .3, .3, .3 });

				material.BaseColor = { 0.8f, CCE::Math::CCMath::Clamp01(1.f / 16.f * (float)y), 0.2f};

				collider.Initialize(entity.Id);
				collider.Radius = transform.Scale().x;

				mesh = Mesh(Application::Instance->resourceDataPath.Path() + "/models/sphere.fbx");

				String pixelShaderPath = Application::Instance->resourceDataPath.Path() + "/shader/DefaultPixelShader.cso";
				String diffuseTexFilePath = Application::Instance->resourceDataPath.Path() + "/models/textures/DefaultMaterial_albedo.jpeg";
				String normalTexFilePath = Application::Instance->resourceDataPath.Path() + "/models/textures/DefaultMaterial_normal.jpeg";

				material.AddBind(std::make_shared<PixelShader>(StringConverter::StringToWString(pixelShaderPath.Value())));
				//material.AddBind(std::make_shared<Texture2D>(diffuseTexFilePath));
				//material.AddBind(std::make_shared<Texture2D>(normalTexFilePath, 1));
				material.AddBind(std::make_shared<Sampler>(D3D11_TEXTURE_ADDRESS_WRAP));

				// Initialize const buffer for each entity!
				mesh.CreateConstBufs(std::move(VSConstBufData(transform.GetTransformationMatrix(), material.BaseColor)));
				bounciness += 0.1f;
			}
		}

		// Create plane

		ECS::Entity entity = ECS::EntityComponentSystem::Instance->CreateEntity();
		RenderPipeline::Instance->RenderingSystem.RegisterEntity(static_cast<long long>(entity.Id));
		Application::Instance->mPhysicsSystem.RegisterEntity(static_cast<long long>(entity.Id));

		auto& rigidbody = entity.AddComponent<Rigidbody>();
		auto& transform = entity.AddComponent<Transform>();
		auto& mesh = entity.AddComponent<Mesh>();
		auto& material = entity.AddComponent<Material>();
		auto& collider = entity.AddComponent<BoxCollider>();

		transform.SetTranslation({ 4, 0, 0 });
		transform.SetScale({ 5, .5, 2 });

		material.BaseColor = { 1,1,1 };

		rigidbody.useGravity = false;
		rigidbody.bounciness = 0.0f;
		rigidbody.mass = 0;				// Non-movable objects have mass = 0

		collider.Initialize(entity.Id);
		collider.Width = transform.Scale().x;
		collider.Height = transform.Scale().y;
		collider.Length = transform.Scale().z;

		mesh = Mesh(Application::Instance->resourceDataPath.Path() + "/models/cube.fbx");

		String pixelShaderPath = Application::Instance->resourceDataPath.Path() + "/shader/DefaultPixelShader.cso";
		String diffuseTexFilePath = Application::Instance->resourceDataPath.Path() + "/models/textures/DefaultMaterial_albedo.jpeg";
		String normalTexFilePath = Application::Instance->resourceDataPath.Path() + "/models/textures/DefaultMaterial_normal.jpeg";

		material.AddBind(std::make_shared<PixelShader>(StringConverter::StringToWString(pixelShaderPath.Value())));
		//material.AddBind(std::make_shared<Texture2D>(diffuseTexFilePath));
		//material.AddBind(std::make_shared<Texture2D>(normalTexFilePath, 1));
		material.AddBind(std::make_shared<Sampler>(D3D11_TEXTURE_ADDRESS_WRAP));

		// Initialize const buffer for each entity!
		mesh.CreateConstBufs(std::move(VSConstBufData(transform.GetTransformationMatrix(), material.BaseColor)));
	}


	/// <summary>
	/// Updates scene by referring to the ECS to update the registered components
	/// </summary>
	void Scene::UpdateScene()
	{
		// @TODO: Update all components (NOT THE ENTITIES!!)
		// Later this means updating the script behaviour for example

		// @TODO: Maybe I don't even need a Scene as its own class
		// The different systems aren't centralized and therefore also 
		// cannot really be aggregated together!
	}

	void Scene::ResetScene()
	{
		using namespace ECS::Components;
		// Do reset here!
		float y = 4.f;
		float x = 0.0f;
		float z = -1.0f;
		float bounciness = 0.9f;
		for (auto& entity : entities)
		{
			entity.GetComponent<Rigidbody>()->bounciness = bounciness;
			entity.GetComponent<Rigidbody>()->mass = 1.0f;
			entity.GetComponent<Rigidbody>()->useGravity = true;
			entity.GetComponent<Rigidbody>()->angularVelocity = { 0, 0, 0 };
			entity.GetComponent<Rigidbody>()->inertiaTensor = {};
			entity.GetComponent<Rigidbody>()->velocity = { 0, 0, 0 };
			entity.GetComponent<Rigidbody>()->acceleration = { 0, 0, 0 };
			entity.GetComponent<Transform>()->SetTranslation({ x += .25f, y++, z += .25f });
			entity.GetComponent<Transform>()->SetRotation({0, 0, 0});
		}
	}

	/// <summary>
	/// Adds a new entity to the scene by inserting it in the scene entity set.
	/// It can therefore be associatedwith this scene.
	/// </summary>
	/// <returns>A reference to the added entity.</returns>
	ECS::Entity& Scene::AddEntity()
	{
		using ECS = ECS::EntityComponentSystem;
		auto entity = ECS::Instance->CreateEntity();
		entities.insert(entity);
		return entity;		// @TODO: Fix this, don't return a local...
	}

	/// <summary>
	/// Removes the referenced entity from the scene and the whole ECS.
	/// This means all it's components are destroyed and given back to the ECS.
	/// </summary>
	/// <param name="entity">The entity to destroy.</param>
	void Scene::RemoveEntity(ECS::Entity& entity)
	{
		using ECS = ECS::EntityComponentSystem;
		ECS::Instance->DestroyEntity(entity);
		entities.erase(entity);
	}

	/// <summary
	/// Is updated by the input system.
	/// </summary>
	/// <param name="mouse"></param>
	/// <param name="keyboard"></param>
	/// <param name="controller"></param>
	void Scene::InputCallback(const Input::Mouse* mouse, const Input::Keyboard* keyboard, const Input::Controller* controller)
	{
		using namespace Input;
		if (keyboard->keys[(int)InputDevice::Keycode::KEY_R] == Keyboard::ButtonState::PRESSED ||
			controller->REast == InputDevice::ButtonState::JUST_PRESSED)
		{
			ResetScene();
		}
	}
}
