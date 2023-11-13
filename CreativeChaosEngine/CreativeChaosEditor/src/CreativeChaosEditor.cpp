#include "CreativeChaosEditor.h"
#include "CCE/ClientWindow/ClientWindow.h"
#include <functional>

#include "GUIDrawables/Base/IGUIDrawable.h"
#include "GUIDrawables/Base/EditorWindow.h"
#include "GUIDrawables/MenuBar.h"
#include "GUIDrawables/RenderingDebugger.h"
#include "GUIDrawables/MemoryWindow.h"
#include "GUIDrawables/JobWindow.h"
#include "GUIDrawables/Inspector.h"
#include "GUIDrawables/InputWindow.h"

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

    Application mRuntimeManager = CCE::Application();

#ifdef DEBUG

    // Conduct unit tests by opening the application with '-test'
    bool unittesting = false;
    for (int i = 0; i < argc; i++)
    {
        std::string arg = std::string(argv[i]);

        LOG("%s", argv[i]);

        if (arg == "-test")
        {
            unittesting = true;
        }
    }

    {
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

    mRuntimeManager.StartUp();


    // ------ OPEN ENGINE WINDOW ------

    {
        // EditorGUI
        MenuBar menuBar = MenuBar();
        RenderingDebugger rendEditorWin = RenderingDebugger("Rendering");
        //MemoryWindow memEditorWin = MemoryWindow("Memory");
        Inspector inspector = Inspector("Inspector");
        InputWindow input = InputWindow("Input");

        // Editor viewport camera
        //CCE::Graphics::Camera viewportCamera = CCE::Graphics::Camera();

#if MULTITHREADED
        JobWindow jobWin = JobWindow("Jobs");
#endif

        using namespace CCE;

        // update window input
        int rValue = 0;

        // ----------------------------------------

        // TODO: Miltithread the editor loop as well
        while (rValue != (int)WM_QUIT)
        {
            mRuntimeManager.PreEditorUpdate(rValue);
            
            //viewportCamera.Update();

            IGUIDrawable::PreGUIUpdate();

            for (int i = 0; i < IGUIDrawable::GetGUIDrawablePtrs().size(); i++)
            {
                IGUIDrawable::GetGUIDrawablePtrs().at(i)->UpdateDrawable();
            }

            IGUIDrawable::PostGUIUpdate();
           
            mRuntimeManager.PostEditorUpdate();
        }
    }

    // ------ SHUTDOWN MANAGER ------

    mRuntimeManager.ShutDown();

    // ------ BYE ------

    LOGC("Shutting down %s", COLOR_BLUE, EDITOR_VERSION);
    Sleep(500);
    
    return 0;
}
