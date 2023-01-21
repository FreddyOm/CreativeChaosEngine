#include <iostream>
#include <CCEngine.h>
#ifdef NDEBUG
#include <Windows.h>
#endif // 


int main(int argc, char* argv[])
{  
    // ------ HELLO ------
    LOGC("Starting Creative Chaos Engine - v0.1", COLOR_BLUE);
    //TODO: Load config file

    CCE::JobManager mJobManager = CCE::JobManager();
    CCE::ProfilingManager mProfilingManager = CCE::ProfilingManager();
    CCE::RenderManager mRenderManager = CCE::RenderManager();
    CCE::PhysicsManager mPhysicsManager = CCE::PhysicsManager();


    // ------ UNIT TESTING ------
#ifdef DEBUG
    LOGC("----------- UNIT TESTS -----------", COLOR_BLUE);
    CCE_Testing::UnitTestMath mathTest;
    CCE_Testing::UnitTestStackAlloc stallocTest;

    mathTest.Test();
    mathTest.Cleanup();
    stallocTest.Test();
    stallocTest.Cleanup();

    LOGC("----------------------------------", COLOR_BLUE);
#endif // Only test when in debug mode


    // ------ STARTUP MANAGER ------

    mJobManager.StartUp();
    mProfilingManager.StartUp();
    mRenderManager.StartUp();
    mPhysicsManager.StartUp();
    

    // ------ RUN LOOP ------

    //CCE::PhysicsManager* pManager = 
      //  (CCE::PhysicsManager*) malloc(sizeof(pManager));
    //pManager = new(pManager)CCE::PhysicsManager();
    //pManager->StartUp();

    std::cin.get(); // Placeholder to prevent the window from closing


    // ------ SHUTDOWN MANAGER ------

    mPhysicsManager.ShutDown();
    mRenderManager.ShutDown();
    mProfilingManager.ShutDown();
    mJobManager.ShutDown();

    // ------ BYE ------
    LOGC("Shutting down Creative Chaos Engine - v0.1", COLOR_BLUE);
    Sleep(2000);
    
    return 0;
}