#include "application.h"
#include "../core.h"
#include "../analysis/time.h"
#include "../analysis/debug.h"
#include "../analysis/logger.h"
#include "../graphics/rendering/D3D11/d3d11RenderPipeline.h"
#include "../client-window/client-window.h"
#include "../multithreading/job-system.h"
#include "../../thirdparty/src/optick.h"
#include "../editor/job-profiler/jobProfiler.h"

namespace CCE
{
	using namespace Jobs;

	/// <summary>
	/// Starts up the application.
	/// </summary>
	void Application::StartUp()
	{
		DASSERT(Instance == nullptr, "Application was instantiated more than once!");
		Instance = this;
		Initialize();

#ifdef CCE_PLATFORM_WINDOWS
		// Load engine config
		if (File::Exists(engineConfig.Path().Value()))
		{
			std::string config = IO::ReadText(engineConfig).Value();
			//Graphics::g_RenderPipelineConfig.DeserializeFromString(config);
		}
#else
#error CCE is currently only supported for Windows
#endif
		startTime = Time::Now();
		initialized = true;
		LOGC("RuntimeManager initialized!", COLOR_BLUE);
	}

	/// <summary>
	/// Shuts down the aplication.
	/// </summary>
	void Application::ShutDown()
	{
		LOGC("Shutting down Application...", COLOR_BLUE);
		if (!initialized) { return; }
		initialized = false;

		// Save engine config
		DASSERT(IO::WriteText(engineConfig,
			Graphics::g_RenderPipelineConfig.SerializeToString(true).c_str(), true),
			"Failed writing engine config to file.");
		
		Graphics::DeinitializeD3D11();
		delete scene;
		delete window;

		// Show leak info
		PRINT_LEAK_INFO;
		Deinitialize();
		Instance = nullptr;

#ifdef DEBUG_PROFILE
		OPTICK_SHUTDOWN();
#endif
	}

	/// <summary>
	/// A static reference to this subsystem.
	/// </summary>
	Application* Application::Instance = nullptr;

	/// <summary>
	/// Run one frame and execute logic before the editor is updated.
	/// </summary>
	void Application::PreEditorUpdate(int& rValue, bool handleInput)
	{
		OPTICK_FRAME("MainThread");
		SAMPLE_JOB();

		if (!m_pause)
		{
			frameBegin = Time::Now();
			Debug::frameStart = frameBegin;
		}

		// Handle input
		window->UpdateClientWindow(reinterpret_cast<uintptr_t>(&rValue)); // @TODO: Check if this can be done by another thread!		
		
		cntPreEditorUpdate.store(handleInput ? 3 : 1, std::memory_order_release);

		if (handleInput)
		{	
			Job inputJobs[2] =
			{
				JOB(Input::FinalizeWinInput, &cntPreEditorUpdate, Priority::NORMAL),
				JOB(Input::HandleXInput, &cntPreEditorUpdate, Priority::NORMAL),
				//Input::HandleDirectInput();
				//Input::HandleDualSenseInput();
			};
			KickJobs(&inputJobs[0], 2);

			BusyWaitForCounter(&cntPreEditorUpdate, 1);
		}

		// @TODO: Replace with NVIDIA PhysX
		//mPhysicsSystem.UpdateSystem();

		Graphics::BeginFrame(reinterpret_cast<uintptr_t>(&Graphics::g_RenderPipelineConfig.backgroundColor));
	}


	void Application::PostEditorUpdate()
	{
		OPTICK_EVENT();
		SAMPLE_JOB();

		cntPostEditorUpdate.store(2, std::memory_order_release);

		Job postEditorUpdateJobs[2] =
		{
			JOB(Graphics::EndFrame, &cntPostEditorUpdate, Priority::LOW),
			JOB(Input::ResetInputValues, &cntPostEditorUpdate, Priority::LOW)
		};
		
		KickJobs(&postEditorUpdateJobs[0], 2);
		BusyWaitForCounter(&cntPostEditorUpdate);

		frameEnd = Time::Now();
		Time::SetDeltaTime(Time::GetDurationInMilliSec(frameBegin, frameEnd));
	}

	bool Application::IsPaused() const
	{
		return m_pause;
	}

	void Application::Pause()
	{
		m_pause = true;
	}

	void Application::Resume()
	{
		m_pause = false;
	}

	/// <summary>
	/// Initialize all engine subsystems.
	/// </summary>
	void Application::Initialize() 
	{
		OPTICK_EVENT();

#ifdef CCE_PLATFORM_WINDOWS

		persistentDataPath = GetPersistentDataPath();
		applicationDataPath = GetApplicationDataPath();
		resourceDataPath = applicationDataPath.Path() + String("/resources");

		// Set engine config file location to persistenDataPath + engine config name and suffix
		engineConfig = persistentDataPath.Path() + String("/config.cce");

		std::string eC = engineConfig.Path().Value();
#else
#error CCE is currently only supported for Windows
#endif
		InitializeLogger();
		mMemoryManager.StartUp();
		Jobs::InitializeThreadpool();
		Input::Initialize();
		mECS.StartUp();
		mPhysicsSystem.StartUp();

		window = new ClientWindow();
		window->OpenWindow(GetModuleHandle(NULL));

		scene = new Scene::Scene();
		scene->SetupScene();
	}

	/// <summary>
	/// Deinitialize all engine subsystems.
	/// </summary>
	void Application::Deinitialize()
	{
		OPTICK_EVENT();

		mPhysicsSystem.ShutDown();
		mECS.ShutDown();
		Input::Deinitialize();

		mMemoryManager.ShutDown();
		Jobs::DeinitializeThreadpool();
		DeinitializeLogger();
	}

	Directory Application::GetPersistentDataPath() const
	{
		OPTICK_EVENT();

#ifdef CCE_PLATFORM_WINDOWS
		std::string persDataPath;
		Directory persDir;

		CHAR szPath[MAX_PATH];
		if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPath)))
		{
			persDataPath =
				std::string(szPath).append("/").append(companyName.Value()).c_str();
		}
		else
		{
			DERROR("Couldn't read the persistent data path!");
		}
		// @TODO: Fix this, this is awful...
		persDir = Directory(strdup(persDataPath.c_str()));
		if (!Directory::Exists(persDataPath.c_str()))
		{
			Directory::Create(persDir.Path());
		}

		return persDir;

#else
#error CCE is currently only supported for Windows
#endif
	}

	Directory Application::GetApplicationDataPath() const
	{
		OPTICK_EVENT();
#ifdef CCE_PLATFORM_WINDOWS
		char pBuf[256] = {};
		ZeroMemory(&pBuf[0], sizeof(pBuf));

		int bytes = GetModuleFileNameA(NULL, pBuf, sizeof(pBuf));
		if (bytes >= sizeof(pBuf))
		{ DERROR("The application data path could be invalid due to buffer overflow."); }

		std::string appDataPath = std::string(std::move(&pBuf[0]), bytes);

		std::string::size_type pos = appDataPath.find_last_of("\\");
		appDataPath = appDataPath.substr(0, pos);

		return Directory(strdup(appDataPath.c_str()));
#else
#error CCE is currently only supported for Windows
#endif	
	}
}
