#pragma once
#include "../Core.h"
#include "BaseManager.h"
#include "InputManager.h"
#include "MemoryManager.h"
#include "PhysicsManager.h"
#include "ProfilingManager.h"
#include "JobManager.h"
#include "../ClientWindow/ClientWindow.h"

namespace CCE
{
	struct CCE_API RuntimeManager : public BaseManager
	{

		RuntimeManager() = default;
		~RuntimeManager() = default;

		void StartUp() override;
		void ShutDown() override;

		static RuntimeManager* Instance;

		void PreEditorUpdate(int& rValue);
		void PostEditorUpdate();

	private:

		void Initialize();
		void Deinitialize();


	private:

		JobManager mJobManager = CCE::JobManager();
		ProfilingManager mProfilingManager = CCE::ProfilingManager();
		PhysicsManager mPhysicsManager = PhysicsManager();
		InputManager mInputManager = InputManager();
		MemoryManager mMemoryManager = MemoryManager();

		ClientWindow window = ClientWindow();

		std::chrono::steady_clock::time_point frameBegin;
		std::chrono::steady_clock::time_point frameEnd;
	
		JobManager::Counter cnt;
	};
}
