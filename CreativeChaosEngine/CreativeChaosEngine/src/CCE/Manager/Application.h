#pragma once
#include "../core.h"
#include "baseManager.h"
#include "./../input/input.h"
#include "memoryManager.h"
#include "profilingManager.h"
#include "../scene/scene.h"
#include "../utilities/io/io.h"
#include "../ecs/entity-component-system.h"
#include "../ecs/systems/physics-system.h"
#include "../ecs/systems/rendering-system.h"
#include "../utilities/serialization/ISerializable.h"
#include "../resources/resource-allocator.h"

namespace CCE
{
	class ClientWindow;
	class CCE_API Application
	{
	public:

		Application() = default;
		~Application() = default;

		void StartUp();
		void ShutDown();

		static Application* Instance;

		void PreEditorUpdate(int& rValue, bool handleInput);
		void PostEditorUpdate();

		bool IsPaused() const;
		void Pause();
		void Resume();

	public:

		String companyName = "CreativeChaosEngine";
		Directory persistentDataPath = {};
		Directory applicationDataPath = {};
		Directory resourceDataPath = {};
		//...

		Time::point startTime{};

	private:

		void Initialize();
		void Deinitialize();
		Directory GetPersistentDataPath() const;
		Directory GetApplicationDataPath() const;

	public:

		ProfilingManager mProfilingManager = CCE::ProfilingManager();
		MemoryManager mMemoryManager = MemoryManager();
		ECS::EntityComponentSystem mECS = ECS::EntityComponentSystem();

		// ECS Systems
		Resources::ResourceAllocator allocator = Resources::ResourceAllocator();
		ECS::Systems::PhysicsSystem mPhysicsSystem = ECS::Systems::PhysicsSystem();
		ECS::Systems::RenderingSystem mRenderingSystem = ECS::Systems::RenderingSystem();

		ClientWindow* window = nullptr;
		Scene::Scene* scene = nullptr;

		std::chrono::steady_clock::time_point frameBegin;
		std::chrono::steady_clock::time_point frameEnd;
	
		Jobs::Counter cntPreEditorUpdate;
		Jobs::Counter cntPostEditorUpdate;

		File engineConfig = {};

		bool initialized = false;

	private:

		bool m_pause = false;
	};

	struct CCE_API EngineConfig : private ISerializable<EngineConfig>
	{
	public:
		bool placeholder = false;

		std::string SerializeToString(bool prettyPrint = false) const override
		{
			JSON data;

			SERIALIZE_CLASS_MEMBER(placeholder);
			std::string out = prettyPrint ? data.dump(4).c_str() : data.dump().c_str();
			return out.c_str();
		}

		std::vector<uint8_t> SerializeToBinary() const override
		{
			JSON data;

			SERIALIZE_CLASS_MEMBER(placeholder);
			return JSON::to_bson(data);
		}

		void DeserializeFromString(std::string serializeString) override
		{
			JSON data = JSON::parse(serializeString);
			DESERIALIZE_CLASS_MEMBER(placeholder);
		}

		void DeserializeFromBinary(std::vector<uint8_t> serializeData) override
		{
			JSON data = JSON::from_bson(serializeData);			
			DESERIALIZE_CLASS_MEMBER(placeholder);
		}
	};
}
