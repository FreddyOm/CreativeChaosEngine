#pragma once
#include "../Core.h"
#include "BaseManager.h"
#include "InputManager.h"
#include "MemoryManager.h"
#include "PhysicsManager.h"
#include "ProfilingManager.h"
#include "JobManager.h"
#include "../Utilities/IO/IO.h"
#include "../Utilities/Serialization/ISerializable.h"

namespace CCE
{
	struct ClientWindow;
	struct CCE_API Application
	{
	public:

		Application() = default;
		~Application() = default;

		void StartUp();
		void ShutDown();

		static Application* Instance;

		void PreEditorUpdate(int& rValue, bool handleInput);
		void PostEditorUpdate();

	public:

		String companyName = "CreativeChaosEngine";
		Directory persistentDataPath;
		Directory applicationDataPath;
		Directory resourceDataPath;
		//...

	private:

		void Initialize();
		void Deinitialize();
		Directory GetPersistentDataPath() const;
		Directory GetApplicationDataPath() const;

	private:

		Jobs::JobManager mJobManager = CCE::Jobs::JobManager();
		ProfilingManager mProfilingManager = CCE::ProfilingManager();
		PhysicsManager mPhysicsManager = PhysicsManager();
		InputManager mInputManager = InputManager();
		MemoryManager mMemoryManager = MemoryManager();

		ClientWindow *window = nullptr;

		std::chrono::steady_clock::time_point frameBegin;
		std::chrono::steady_clock::time_point frameEnd;
	
		Jobs::JobManager::Counter cnt;

		File engineConfig;

		UINT64 maxUsedFibersPerFrame = 0;
		bool initialized = false;
	};

	struct CCE_API EngineConfig : private ISerializable<EngineConfig>
	{
	public:
		bool multithreaded = false;

		std::string SerializeToString(bool prettyPrint = false) override
		{
			JSON data;

			SERIALIZE_CLASS_MEMBER(multithreaded);
			std::string out = prettyPrint ? data.dump(4).c_str() : data.dump().c_str();
			return out.c_str();
		}

		std::vector<uint8_t> SerializeToBinary() override
		{
			JSON data;

			SERIALIZE_CLASS_MEMBER(multithreaded);
			return JSON::to_bson(data);
		}

		void DeserializeFromString(std::string serializeString) override
		{
			JSON data = JSON::parse(serializeString);
			DESERIALIZE_CLASS_MEMBER(multithreaded);
		}

		void DeserializeFromBinary(std::vector<uint8_t> serializeData) override
		{
			JSON data = JSON::from_bson(serializeData);			
			DESERIALIZE_CLASS_MEMBER(multithreaded);
		}
	};
}
