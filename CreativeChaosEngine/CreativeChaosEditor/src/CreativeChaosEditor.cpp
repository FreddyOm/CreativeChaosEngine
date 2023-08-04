#include "CreativeChaosEditor.h"
#include "CCE/ClientWindow/ClientWindow.h"
#include <functional>

#include "EditorWindow/Base/EditorWindow.h"
#include "EditorWindow/RenderingDebugger.h"
#include "EditorWindow/MemoryWindow.h"
#include "EditorWindow/JobWindow.h"

// -------- Testing ---------

#ifdef DEBUG

#include "CCE/Analysis/UnitTesting/UnitTest.h"
#include "CCE/Analysis/UnitTesting/UnitTestMath.h"
#include "CCE/Analysis/UnitTesting/UnitTestStackAlloc.h"
#include "CCE/Analysis/UnitTesting/UnitTestPoolAlloc.h"
#include "CCE/Analysis/UnitTesting/UnitTestString.h"

#endif
#include <CCE/CCEditor/CCEditor.h>

// -------------------------

#define EDITOR_VERSION "Creative Chaos Engine - v0.1"

#define MULTITHREADED 0

using namespace CCE;

int main(int argc, char* argv[])
{
    // ------ HELLO ------
    LOGC("Starting %s", COLOR_BLUE, EDITOR_VERSION);
    //TODO: Load config file

    JobManager mJobManager = CCE::JobManager();
    ProfilingManager mProfilingManager = CCE::ProfilingManager();
    PhysicsManager mPhysicsManager = PhysicsManager();
    InputManager mInputManager = InputManager();
    MemoryManager mMemoryManager = MemoryManager();
    CCE::CCEditor editor = CCE::CCEditor();

    {
#ifdef DEBUG
        // Conduct unit tests by opening the application with '-test'
        bool unittesting = false;
        for(int i = 0; i < argc; i++)
        {
            std::string arg = std::string(argv[i]);
            
            if (arg == std::string("-test"))
            {
                unittesting = true;
            }
        }

        if (unittesting)
        {
            // TODO: Wrap in class / struct
            // TODO: Add - test details to show which tests failed / suceeded
            // and normally show [30 / 30 tests suceeded!]
            // ------ UNIT TESTING 
            CCE::Logger::SetLogLvlMaks(0b00001001);
            LOGC("----------- UNIT TESTS -----------", COLOR_BLUE);
            CCE_Testing::UnitTestStackAlloc stallocTest;
            CCE_Testing::UnitTestPoolAlloc poolallocTest;
            CCE_Testing::UnitTestString stringTest;

            stallocTest.Test();
            stallocTest.Cleanup();
            poolallocTest.Test();
            poolallocTest.Cleanup();
            stringTest.Test();
            stringTest.Cleanup();

            LOGC("----------------------------------", COLOR_BLUE);
        }
#endif // Only test when in debug mode
    }
    
    // ------ STARTUP MANAGER ------

    mMemoryManager.StartUp();
    mJobManager.StartUp();
    mProfilingManager.StartUp();
    mPhysicsManager.StartUp();
    mInputManager.StartUp();


    // ------ OPEN ENGINE WINDOW ------

    {
        ClientWindow window = ClientWindow();
        window.OpenWindow(GetModuleHandle(NULL));

        // EditorWindows
        RenderingDebugger rendEditorWin = RenderingDebugger("Rendering");
        MemoryWindow memEditorWin = MemoryWindow("Memory");
        JobWindow jobWin = JobWindow("Jobs");


        using namespace CCE;

        // update window input
        int rValue = 0;

#if MULTITHREADED    
        JobManager::EntryPoint handleXInput = BIND(mInputManager.HandleXInput);
        JOBDECL declHandleXInput = JOBDECL(handleXInput,JobManager::Priority::NORMAL);

        JobManager::EntryPoint beginFrame = BIND(window.GetRenderPipeline()->BeginFrame,
            window.GetRenderPipeline()->GetRenderPipelineConfig()->backgroundColor);
        JOBDECL declBeginFrame;
        declBeginFrame += beginFrame;

        JobManager::EntryPoint endFrame = BIND(window.GetRenderPipeline()->EndFrame);
        JOBDECL declEndFrame;
        declEndFrame += endFrame;

        JobManager::EntryPoint updateClientWin = BIND(window.UpdateClientWindow, rValue);
        JOBDECL declUpdateEditorWin;
        declUpdateEditorWin += updateClientWin;

        bool initialized = false;
        // Update loop
        JobManager::Counter cnt = JobManager::Counter(2);
        
#endif

        // ----------------------------------------

        while (rValue != (int)WM_QUIT)
        {
            auto start = Time::Now();
#if MULTITHREADED
            cnt = 3;

            // Update Client Window
            mJobManager.KickJob(&declUpdateEditorWin, &cnt);
            mJobManager.WaitForCounter(&cnt, 2);
            // Update GFX
            mJobManager.KickJob(&declBeginFrame, &cnt);
            mJobManager.WaitForCounter(&cnt, 1);

            EditorWindow::PreGUIUpdate();

            // Update Inputs
            mInputManager.HandleXInput();

            // TODO: Kick multiple jobs
            for (int i = 0; i < EditorWindow::GetEditorWindowPtrs().size(); i++)
            {
                EditorWindow::GetEditorWindowPtrs().at(i)->UpdateWindow();
            }

            EditorWindow::PostGUIUpdate();
            mJobManager.KickJob(&declEndFrame, &cnt);
            mJobManager.WaitForCounter(&cnt, 0);

#else
            window.UpdateClientWindow(rValue);

            window.GetRenderPipeline()->BeginFrame(window.GetRenderPipeline()->GetRenderPipelineConfig()->backgroundColor);
            EditorWindow::PreGUIUpdate();

            //mInputManager.HandleDirectInput();
            mInputManager.HandleXInput();

            for (int i = 0; i < EditorWindow::GetEditorWindowPtrs().size(); i++)
            {
                EditorWindow::GetEditorWindowPtrs().at(i)->UpdateWindow();
            }

            EditorWindow::PostGUIUpdate();
            window.GetRenderPipeline()->EndFrame();
#endif
            mMemoryManager.UpdateMemoryUsage();
            auto end = Time::Now();
            Time::SetDeltaTime(Time::GetDurationInMilliSec(start, end));
        }
    }

    // HOW TO HANDLE DIFFERENT RETURN TYPES? 

    // ------ SHUTDOWN MANAGER ------

    PRINT_LEAK_INFO;

    mInputManager.ShutDown();
    mPhysicsManager.ShutDown();
    mProfilingManager.ShutDown();
    mJobManager.ShutDown();
    mMemoryManager.ShutDown();

    // ------ BYE ------

    LOGC("Shutting down %s", COLOR_BLUE, EDITOR_VERSION);
    Sleep(500);
    
    return 0;
}
