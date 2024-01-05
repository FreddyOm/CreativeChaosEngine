#include "Application.h"
#include "../Analysis/Time.h"
#include "../Analysis/Debug.h"
#include "../Analysis/Logger.h"
#include "../Graphics/RenderPipeline.h"
#include "../ClientWindow/ClientWindow.h"
#include <functional>

namespace CCE
{
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
			window->GetRenderPipeline()->GetRenderPipelineConfig()->DeserializeFromString(config);
		}
#else
#error CCE is currently only supported for Windows
#endif

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
			window->GetRenderPipeline()->GetRenderPipelineConfig()->SerializeToString(true).c_str(), true),
			"Failed writing engine config to file.");
		
		delete scene;
		delete window;
		// Show leak info
		PRINT_LEAK_INFO;
		Deinitialize();
		Instance = nullptr;
	}

	/// <summary>
	/// A static reference to this subsystem.
	/// </summary>
	Application* Application::Instance = nullptr;

	/// <summary>
	/// Run one frame
	/// </summary>
	void Application::PreEditorUpdate(int& rValue, bool handleInput)
	{
		frameBegin = Time::Now();
#if MULTITHREADED

		if (handleInput)
			mInputManager.FinalizeWinInput();

		window->UpdateClientWindow(rValue);

		cnt = 1;

		JobManager::EntryPoint epRPBF = std::bind(&Graphics::BeginFrame,
			Graphics::RenderPipeline::Instance->GetDeviceContextPtr(),
			Graphics::RenderPipeline::Instance->GetRenderTargetComPtr(),
			Graphics::RenderPipeline::Instance->GetDepthStencilViewPtr(),
			Graphics::RenderPipeline::Instance->pViewportCamera,
			Graphics::RenderPipeline::Instance->testModels,
			window->GetRenderPipeline()->GetRenderPipelineConfig()->backgroundColor);

		JOBDECL declBeginFrame = JOBDECL(epRPBF, JobManager::Priority::LOW);

		mJobManager.KickJob(declBeginFrame, &cnt);

		// Make sure not to busy wait on main thread!
		// Otherwise, the main thread waits for execution and 
		// puts the main fiber onto the waitlist infinitely!!
		while (cnt != 0)
		{
			continue;
		}

		/*
		JobManager::EntryPoint epHXI = BIND(mInputManager.HandleXInput);
		JOBDECL declHandleInput = JOBDECL(epHXI, JobManager::Priority::LOW);


		mJobManager.KickJobAndFreeDecl(declBeginFrame, &cnt);
		mJobManager.KickJobAndFreeDecl(declHandleInput, &cnt);

		window->UpdateClientWindow(rValue);

		mJobManager.BusyWaitForCounter(cnt, 2);
		*/
#else
		window->UpdateClientWindow(rValue);

		if(handleInput)
			mInputManager.FinalizeWinInput();

		window->GetRenderPipeline()->BeginFrame(window->GetRenderPipeline()->GetRenderPipelineConfig()->backgroundColor);

		//Update scene
		scene->UpdateScene();

		//mInputManager.HandleDirectInput();
		mInputManager.HandleXInput();
#endif
	}

	void Application::PostEditorUpdate()
	{
#if MULTITHREADED
		JobManager::EntryPoint epRPEF = BIND(window.GetRenderPipeline()->EndFrame);
		JOBDECL declEndFrame = JOBDECL(epRPEF, JobManager::Priority::LOW);

		JobManager::EntryPoint epUMU = BIND(mMemoryManager.UpdateMemoryUsage);
		JOBDECL declUpdateMemUsage = JOBDECL(epUMU, JobManager::Priority::LOW);


		mJobManager.KickJobAndFreeDecl(declEndFrame, &cnt);
		mJobManager.KickJobAndFreeDecl(declUpdateMemUsage, &cnt);

		mJobManager.BusyWaitForCounter(cnt, 0);

#else
		window->GetRenderPipeline()->EndFrame();
		mInputManager.ResetInputValues();

#endif

		frameEnd = Time::Now();
		Time::SetDeltaTime(Time::GetDurationInMilliSec(frameBegin, frameEnd));
	}

	/// <summary>
	/// Initialize all engine subsystems.
	/// </summary>
	void Application::Initialize() 
	{
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
#if MULTITHREADED
		mJobManager.StartUp();
#endif
		mInputManager.StartUp();
		mECS.StartUp();
		mPhysicsManager.StartUp();

		window = new ClientWindow();
		window->OpenWindow(GetModuleHandle(NULL));

		scene = new Scene::Scene();
	}

	/// <summary>
	/// Deinitialize all engine subsystems.
	/// </summary>
	void Application::Deinitialize()
	{
		mPhysicsManager.ShutDown();
		mECS.ShutDown();
		mInputManager.ShutDown();
#if MULTITHREADED
		mJobManager.ShutDown();
#endif
		mMemoryManager.ShutDown();
	}

	Directory Application::GetPersistentDataPath() const
	{
		std::string persDataPath;
		Directory persDir;
#ifdef CCE_PLATFORM_WINDOWS

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
#else
#error CCE is currently only supported for Windows
#endif

		// @TODO: Fix this, this is awful...
		persDir = Directory(strdup(persDataPath.c_str()));
		if (!Directory::Exists(persDataPath.c_str()))
		{
			Directory::Create(persDir.Path());
		}

		return persDir;
	}

	Directory Application::GetApplicationDataPath() const
	{
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
