#include <iostream>
#include <CCEngine.h>

int main(int argc, char* argv[])
{  
    // ------ HELLO ------
    LOGC("Starting Creative Chaos Engine - v0.1", COLOR_BLUE);
    //TODO: Load config file

    CCE::JobManager mJobManager = CCE::JobManager();
    CCE::ProfilingManager mProfilingManager = CCE::ProfilingManager();
    CCE::RenderManager mRenderManager = CCE::RenderManager();
    CCE::PhysicsManager mPhysicsManager = CCE::PhysicsManager();
    CCE::TestManager mTestManager = CCE::TestManager();

    // ------ STARTUP MANAGER ------

    mJobManager.StartUp();
    mProfilingManager.StartUp();
    mRenderManager.StartUp();
    mPhysicsManager.StartUp();
    mTestManager.StartUp();
    

    // ------ RUN LOOP ------

    std::cin.get(); // Placeholder to prevent the window from closing


    // ------ SHUTDOWN MANAGER ------

    mTestManager.ShutDown();
    mPhysicsManager.ShutDown();
    mRenderManager.ShutDown();
    mProfilingManager.ShutDown();
    mJobManager.ShutDown();


    // ------ BYE ------
    LOGC("Shutting down Creative Chaos Engine - v0.1", COLOR_BLUE);
    Sleep(2000);
    
    return 0;
}