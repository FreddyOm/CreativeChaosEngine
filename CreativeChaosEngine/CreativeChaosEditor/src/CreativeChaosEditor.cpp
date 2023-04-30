#include "CreativeChaosEditor.h"
#include "EditorWindow/EditorWindow.h"
#include "LoadTest.h"
#include "CCE/Manager/JobManager.h"
#include <functional>

// -------- Testing ---------

#ifdef DEBUG

#include "CCE/Analysis/UnitTesting/UnitTest.h"
#include "CCE/Analysis/UnitTesting/UnitTestMath.h"
#include "CCE/Analysis/UnitTesting/UnitTestStackAlloc.h"
#include "CCE/Analysis/UnitTesting/UnitTestPoolAlloc.h"
#include "CCE/Analysis/UnitTesting/UnitTestString.h"

#endif

// -------------------------

#define EDITOR_VERSION "Creative Chaos Engine - v0.1"

#define MULTITHREADED 1


int main(int argc, char* argv[])
{
    // ------ HELLO ------
    LOGC("Starting %s", COLOR_BLUE, EDITOR_VERSION);
    //TODO: Load config file

    CCE::JobManager mJobManager = CCE::JobManager();
    CCE::ProfilingManager mProfilingManager = CCE::ProfilingManager();
    CCE::PhysicsManager mPhysicsManager = CCE::PhysicsManager();
    CCE::InputManager mInputManager = CCE::InputManager();
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

    mJobManager.StartUp();
    mProfilingManager.StartUp();
    mPhysicsManager.StartUp();
    mInputManager.StartUp();

    // ------ OPEN ENGINE WINDOW ------

    CCE::Color backgroundColor = CCE::Color("#F4C167");

    {
        EditorWindow window = EditorWindow(&mInputManager);
        window.OpenWindow(GetModuleHandle(NULL));

        using namespace CCE;

        /*
        JobManager::EntryPoint handleXInput = 
            BIND(InputManager::HandleXInput, mInputManager);
        JOBDECL declHandleXInput = JOBDECL(handleXInput,JobManager::Priority::HIGH);
        
        // ----------------------------------------

        JobManager::EntryPoint beginFrame =
            BIND(Graphics::RenderPipeline::BeginFrame, window.GetRenderPipeline(), backgroundColor);
        JOBDECL declBeginFrame = JOBDECL(beginFrame, JobManager::Priority::HIGH);

        JobManager::EntryPoint endFrame =
            BIND(Graphics::RenderPipeline::EndFrame, window.GetRenderPipeline());
        JOBDECL declEndFrame = JOBDECL(endFrame, JobManager::Priority::HIGH);

        // update window input
        int rValue = 0;

        JobManager::EntryPoint updateEditorWin =
            BIND(EditorWindow::UpdateEditorWindow, window, rValue);
        JOBDECL declUpdateEditorWin = JOBDECL(updateEditorWin, JobManager::Priority::HIGH);

        bool initialized = false;
        // Update loop

        */

        /*
        while (true)
        {
            // TODO: Handle inputs on different thread (-> via job system)
            // update controller input
            //mInputManager.HandleXInput();
            mJobManager.KickJob(declHandleXInput);
            //mInputManager.HandleDirectInput();


            mJobManager.KickJob(declUpdateEditorWin);

            //window.UpdateEditorWindow(rValue);
            if (rValue == (int)WM_QUIT) { break; }
            
            JobManager::Counter pCnt = JobManager::Counter();
            pCnt = 1;

            // update gfx
            mJobManager.KickJob(declBeginFrame, &pCnt);
            mJobManager.WaitForCounterAndFree(&pCnt, 0);
            mJobManager.KickJob(declEndFrame);
            //window.GetRenderPipeline()->BeginFrame(backgroundColor);
            //window.GetRenderPipeline()->EndFrame();
        }
        */

        JobManager::Counter* cnt = new JobManager::Counter(4);

        JobManager::EntryPoint one =
            BIND(ProfilingManager::One, mProfilingManager);
        JOBDECL declOne =
            JOBDECL(one, JobManager::Priority::HIGH);

        JobManager::EntryPoint two =
            BIND(ProfilingManager::Two, mProfilingManager);
        JOBDECL declTwo =
            JOBDECL(two, JobManager::Priority::LOW);

        JobManager::EntryPoint three =
            BIND(ProfilingManager::Three, mProfilingManager);
        JOBDECL declThree =
            JOBDECL(three, JobManager::Priority::HIGH);

        JobManager::EntryPoint four =
            BIND(ProfilingManager::Four, mProfilingManager);
        JOBDECL declFour =
            JOBDECL(four, JobManager::Priority::HIGH);

        while (true)
        {
            auto start = Time::Now();

#if MULTITHREADED == 1

            *cnt = 16;

            mJobManager.KickJob(&declOne, cnt);
            mJobManager.KickJob(&declTwo, cnt);
            mJobManager.KickJob(&declThree, cnt);
            mJobManager.KickJob(&declFour, cnt);
            mJobManager.KickJob(&declOne, cnt);
            mJobManager.KickJob(&declTwo, cnt);
            mJobManager.KickJob(&declThree, cnt);
            mJobManager.KickJob(&declFour, cnt);
            mJobManager.KickJob(&declOne, cnt);
            mJobManager.KickJob(&declTwo, cnt);
            mJobManager.KickJob(&declThree, cnt);
            mJobManager.KickJob(&declFour, cnt);
            mJobManager.KickJob(&declOne, cnt);
            mJobManager.KickJob(&declTwo, cnt);
            mJobManager.KickJob(&declThree, cnt);
            mJobManager.KickJob(&declFour, cnt);

            // This makes sure every job for this frame was finished!
            mJobManager.WaitForCounter(cnt, 0);

#else
            mProfilingManager.One();
            mProfilingManager.Two();
            mProfilingManager.Three();
            mProfilingManager.Four();
            mProfilingManager.One();
            mProfilingManager.Two();
            mProfilingManager.Three();
            mProfilingManager.Four();
#endif

            auto end = Time::Now();
            Time::UpdateDeltaTime(Time::GetDurationInMilliSec(start,end));
            LOG("END OF LOOP. Frametime: %f milliseconds", Time::deltaTime);
            LOG("END OF LOOP. Avg-Frametime: %f milliseconds", Time::GetAverageFrameTime());
        }
    }

    // HOW TO HANDLE DIFFERENT RETURN TYPES? 
    // AND IS IT POSSIBLE TO RUN NON JOBIFIED CODE?
    // OR DOES EVERYTHING HAVE TO BE A JOB?

    // ------ SHUTDOWN MANAGER ------

    mInputManager.ShutDown();
    mPhysicsManager.ShutDown();
    mProfilingManager.ShutDown();
    mJobManager.ShutDown();


    // ------ BYE ------

    LOGC("Shutting down %s", COLOR_BLUE, EDITOR_VERSION);
    Sleep(500);
    
    return 0;
}

void TestLoad()
{

}
