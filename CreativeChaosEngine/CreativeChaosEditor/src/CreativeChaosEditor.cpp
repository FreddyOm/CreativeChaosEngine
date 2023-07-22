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

#define MULTITHREADED 1

using namespace CCE;

int main(int argc, char* argv[])
{
    // ------ HELLO ------
    LOGC("Starting %s", COLOR_BLUE, EDITOR_VERSION);
    //TODO: Load config file

    RuntimeManager mRuntimeManager = CCE::RuntimeManager();

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

    mRuntimeManager.StartUp();


    // ------ OPEN ENGINE WINDOW ------

    {
        // EditorWindows
        RenderingDebugger rendEditorWin = RenderingDebugger("Rendering");
        MemoryWindow memEditorWin = MemoryWindow("Memory");
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

            EditorWindow::PreGUIUpdate();

            for (int i = 0; i < EditorWindow::GetEditorWindowPtrs().size(); i++)
            {
                EditorWindow::GetEditorWindowPtrs().at(i)->UpdateWindow();
            }

            EditorWindow::PostGUIUpdate();
           
            mRuntimeManager.PostEditorUpdate();
        }
    }

    // HOW TO HANDLE DIFFERENT RETURN TYPES? 

    // ------ SHUTDOWN MANAGER ------

    mRuntimeManager.ShutDown();

    // ------ BYE ------

    LOGC("Shutting down %s", COLOR_BLUE, EDITOR_VERSION);
    Sleep(500);
    
    return 0;
}
