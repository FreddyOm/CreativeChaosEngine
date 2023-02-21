#include "CreativeChaosEditor.h"
#include "EditorWindow/EditorWindow.h"

// -------- Testing ---------

#ifdef DEBUG

#include "CCE/Analysis/UnitTesting/UnitTest.h"
#include "CCE/Analysis/UnitTesting/UnitTestMath.h"
#include "CCE/Analysis/UnitTesting/UnitTestStackAlloc.h"
#include "CCE/Analysis/UnitTesting/UnitTestPoolAlloc.h"
#include "CCE/Analysis/UnitTesting/UnitTestString.h"

#endif

// -------------------------


int main(int argc, char* argv[])
{  
    // ------ HELLO ------
    LOGC("Starting Creative Chaos Engine - v0.1", COLOR_BLUE);
    //TODO: Load config file

    CCE::JobManager mJobManager = CCE::JobManager();
    CCE::ProfilingManager mProfilingManager = CCE::ProfilingManager();
    CCE::RenderManager mRenderManager = CCE::RenderManager();
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
    mRenderManager.StartUp();
    mPhysicsManager.StartUp();
    mInputManager.StartUp();

    // ------ OPEN ENGINE WINDOW ------

    {
        EditorWindow window = EditorWindow(&mInputManager);
        window.OpenWindow(GetModuleHandle(NULL));

        // ------ RUN LOOP ------
        
        // TODO: Evaluate if it's nessecary to update the win
        // using a specific method call or if it's also fine to
        // inti the msg pump at the end of OpenWindow (look into
        // child window creation here)
        window.UpdateWindow();
    }

    // ------ SHUTDOWN MANAGER ------

    mInputManager.ShutDown();
    mPhysicsManager.ShutDown();
    mRenderManager.ShutDown();
    mProfilingManager.ShutDown();
    mJobManager.ShutDown();


    // ------ BYE ------

    LOGC("Shutting down Creative Chaos Engine - v0.1", COLOR_BLUE);
    Sleep(500);
    
    return 0;
}