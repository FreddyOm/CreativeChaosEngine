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

		Initialize();

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
		frameBegin = Time::Now();
#if MULTITHREDED
		cnt = 5;

		JobManager::EntryPoint epCWUW = BIND(window.UpdateClientWindow, rValue);
		JOBDECL declUpdateWin = JOBDECL(epCWUW, JobManager::Priority::HIGH, cnt);

		JobManager::EntryPoint epRPBF = BIND(window.GetRenderPipeline()->BeginFrame, 
				window.GetRenderPipeline()->GetRenderPipelineConfig()->backgroundColor);
		JOBDECL declBeginFrame = JOBDECL(epRPBF, JobManager::Priority::HIGH, cnt);

		JobManager::EntryPoint epHXI = BIND(mInputManager.HandleXInput);
		JOBDECL declHandleInput = JOBDECL(epHXI, JobManager::Priority::HIGH, cnt);

		mJobManager.KickJobAndFreeDecl(declUpdateWin, &cnt);
		mJobManager.KickJobAndFreeDecl(declBeginFrame, &cnt);
		mJobManager.KickJobAndFreeDecl(declHandleInput, &cnt);

		mJobManager.WaitForCounter(&cnt, 2);

#else
		window.UpdateClientWindow(rValue);
		window.GetRenderPipeline()->BeginFrame(window.GetRenderPipeline()->GetRenderPipelineConfig()->backgroundColor);

		//mInputManager.HandleDirectInput();
		mInputManager.HandleXInput();
#endif
	}

	void RuntimeManager::PostEditorUpdate()
	{
#if MULTITHREADED
		JobManager::EntryPoint epRPEF = BIND(window.GetRenderPipeline()->EndFrame);
		JOBDECL declEndFrame = JOBDECL(epRPEF, JobManager::Priority::HIGH, cnt);

		JobManager::EntryPoint epUMU = BIND(mMemoryManager.UpdateMemoryUsage);
		JOBDECL declUpdateMemUsage = JOBDECL(epUMU, JobManager::Priority::NORMAL, cnt);


		mJobManager.KickJobAndFreeDecl(declEndFrame, &cnt);
		mJobManager.KickJobAndFreeDecl(declUpdateMemUsage, &cnt);

		mJobManager.WaitForCounter(&cnt, 0);
#else
		window.GetRenderPipeline()->EndFrame();
		mMemoryManager.UpdateMemoryUsage();

#endif
		frameEnd = Time::Now();
		Time::SetDeltaTime(Time::GetDurationInMilliSec(frameBegin, frameEnd));
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
