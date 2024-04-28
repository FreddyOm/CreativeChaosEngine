#include "application.h"
#include "../core.h"
#include "../analysis/time.h"
#include "../analysis/debug.h"
#include "../analysis/logger.h"
#include "../graphics/rendering.h"
#include "../client-window/client-window.h"
#include "../multithreading/job-system.h"
#include "../../thirdparty/src/optick.h"

namespace CCE
{
	using namespace Jobs;

	std::atomic<long long> workCounter = 0;

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
		if (!m_pause)
		{
			frameBegin = Time::Now();
		}

		// Handle input

		window->UpdateClientWindow(rValue); // @TODO: Check if this can be done by another thread!
		if (handleInput)
			mInputManager.FinalizeWinInput();

		//mInputManager.HandleDirectInput();
		mInputManager.HandleXInput();	// @TODO: Check if this can be done by another thread!

		mPhysicsSystem.UpdateSystem();

		/*cnt.store(1, std::memory_order_release);

		Job beginFrameJob(Graphics::BeginFrame, &cnt, Priority::NORMAL,
			reinterpret_cast<uintptr_t>(&Graphics::g_RenderPipelineConfig.backgroundColor));
		KickJob(std::move(beginFrameJob));

		BusyWaitForCounter(&cnt);
		*/
		Graphics::BeginFrame(reinterpret_cast<uintptr_t>(&Graphics::g_RenderPipelineConfig.backgroundColor));
	}

	Jobs::JobReturnType DoWork()
	{
		OPTICK_EVENT();
		++workCounter;
	}

	void Application::TestJobSystem()
	{
		OPTICK_FRAME("MainThread");
		while (cnt != 0)
		{
		}
		cnt.store(100, std::memory_order_release);


		for (int i = 0; i < 100; ++i)
		{
			Job workJob = Job(DoWork, &cnt, Priority::HIGH);
			Jobs::KickJob(std::move(workJob));
		}

		Jobs::BusyWaitForCounter(&cnt);
	}

	void Application::PostEditorUpdate()
	{
		OPTICK_EVENT();
		
		//cnt.store(1, std::memory_order_release);

		//window->GetRenderPipeline()->EndFrame();

		
		//Job endFrameJob = Job(Graphics::EndFrame, &cnt, Priority::LOW);
		//Jobs::KickJob(std::move(endFrameJob));

		//Jobs::BusyWaitForCounter(&cnt);

		Graphics::EndFrame();
		mInputManager.ResetInputValues();

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
		mMemoryManager.StartUp();
		Jobs::InitializeThreadpool(16);
		mInputManager.StartUp();
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
		mInputManager.ShutDown();
#if MULTITHREADED
		//mJobManager.ShutDown();
#endif

		mMemoryManager.ShutDown();
		Jobs::DeinitializeThreadpool();
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
