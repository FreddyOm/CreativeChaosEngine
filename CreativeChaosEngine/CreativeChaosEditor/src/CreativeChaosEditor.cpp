#include <iostream>
#include <CCEngine.h>

int main(int argc, char* argv[])
{  
    // ------ HELLO ------
    LOGC("Starting Creative Chaos Engine - v0.1", COLOR_BLUE);


    CCE::ProfilingManager mProfilingManager = CCE::ProfilingManager();

    // ------ STARTUP MANAGER ------

    mProfilingManager.StartUp();


    // ------ RUN LOOP ------

    std::cin.get(); // Placeholder to prevent the window from closing


    // ------ SHUTDOWN MANAGER ------

    mProfilingManager.ShutDown();


    // ------ BYE ------
    LOGC("Shutting down Creative Chaos Engine - v0.1", COLOR_BLUE);
    Sleep(1000);
    
    return 0;
}