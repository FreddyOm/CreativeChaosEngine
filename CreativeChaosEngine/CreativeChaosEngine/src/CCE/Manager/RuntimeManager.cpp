#include "RuntimeManager.h"
#include "../Analysis/Debug.h"
#include "../Analysis/Logger.h"
#include "../Analysis/Time.h"


namespace CCE
{
	/// <summary>
	/// Starts up the runtime manager.
	/// </summary>
	void RuntimeManager::StartUp()
	{
		DASSERT(Instance == nullptr, "RuntimeManager was instantiated more than once!");

		auto startTime = Time::CurrentTick();

		Initialize();

		auto endTime = Time::CurrentTick();
		double initDuration = Time::GetDurationInMicroSec(startTime, endTime);
		initialized = true;
		LOGC("RuntimeManager initialized!", COLOR_BLUE);
	}

	/// <summary>
	/// Shuts down the runtime manager.
	/// </summary>
	void RuntimeManager::ShutDown()
	{
		if (!initialized) { return; }
		LOGC("Shutting down RuntimeManager...", COLOR_BLUE);
		initialized = false;
		
		Deinitialize();
		
		Instance = nullptr;
	}

	/// <summary>
	/// A static reference to this subsystem.
	/// </summary>
	RuntimeManager* RuntimeManager::Instance = nullptr;

	/// <summary>
	/// Run one frame
	/// </summary>
	void RuntimeManager::PreEditorUpdate(int& rValue)
	{
		start = Time::Now();

		window.UpdateClientWindow(rValue);
		window.GetRenderPipeline()->BeginFrame(window.GetRenderPipeline()->GetRenderPipelineConfig()->backgroundColor);

		//mInputManager.HandleDirectInput();
		mInputManager.HandleXInput();				
	}

	void RuntimeManager::PostEditorUpdate()
	{
		window.GetRenderPipeline()->EndFrame();
		mMemoryManager.UpdateMemoryUsage();

		end = Time::Now();
		Time::SetDeltaTime(Time::GetDurationInMilliSec(start, end));
	}

	/// <summary>
	/// Initialize all engine subsystems.
	/// </summary>
	void RuntimeManager::Initialize() 
	{
		mMemoryManager.StartUp();
		mJobManager.StartUp();
		mProfilingManager.StartUp();
		mPhysicsManager.StartUp();
		mInputManager.StartUp();

		window.OpenWindow(GetModuleHandle(NULL));

	}

	/// <summary>
	/// Deinitialize all engine subsystems.
	/// </summary>
	void RuntimeManager::Deinitialize()
	{
		window.~ClientWindow();

		mInputManager.ShutDown();
		mPhysicsManager.ShutDown();
		mProfilingManager.ShutDown();
		mJobManager.ShutDown();
		mMemoryManager.ShutDown();
	}

}
