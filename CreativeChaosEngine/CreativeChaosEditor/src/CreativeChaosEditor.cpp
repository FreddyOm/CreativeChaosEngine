#include "CreativeChaosEditor.h"
#include "CCE/client-window/client-window.h"
#include <functional>

#pragma region drawables

#include "GUIDrawables/Base/IGUIDrawable.h"
#include "GUIDrawables/Base/EditorWindow.h"
#include "GUIDrawables/RuntimeDebugger.h"
#include "GUIDrawables/RenderingDebugger.h"
#include "GUIDrawables/MemoryWindow.h"
#include "GUIDrawables/JobWindow.h"
#include "GUIDrawables/Inspector.h"
#include "GUIDrawables/InputWindow.h"
#include "GUIDrawables/Console.h"
#include "GUIDrawables/Icons.h"

#pragma endregion drawables

// -------- Testing ---------

#ifdef DEBUG

#include "CCE/analysis/unit-testing/unit-test.h"
#include "CCE/analysis/unit-testing/unit-test-math.h"
#include "CCE/analysis/unit-testing/unit-test-stack-alloc.h"
#include "CCE/analysis/unit-testing/unit-test-pool-alloc.h"
#include "CCE/analysis/unit-testing/unit-test-string.h"

#endif

// -------------------------

#define EDITOR_VERSION "Creative Chaos Engine - v0.1"

#define MULTITHREADED 0

using namespace CCE;

int main(int argc, char* argv[])
{
    // ------ HELLO ------
    LOGC("Starting %s", COLOR_BLUE, EDITOR_VERSION);
    // @TODO: Load config file

    Application mRuntimeManager = CCE::Application();

#ifdef DEBUG

    // Conduct unit tests by opening the application with '-test'
    bool unittesting = false;
    for (int i = 0; i < argc; i++)
    {
        std::string arg = std::string(argv[i]);

        if (arg == "-test")
        {
            unittesting = true;
        }
    }

    {
        if (unittesting)
        {
            // @TODO: Wrap in class / struct
            // @TODO: Add - test details to show which tests failed / suceeded
            // and normally show [30 / 30 tests suceeded!]
            // ------ UNIT TESTING  ------
            
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
    }
#endif // Only test when in debug mode

    // ------ STARTUP MANAGER ------

    mRuntimeManager.StartUp();


    // ------ OPEN ENGINE WINDOW ------

    {
        // EditorGUI
        double imgui_process_time_ms = 0;
        RuntimeDebugger runtimeDebugger = RuntimeDebugger(&imgui_process_time_ms);
        RenderingDebugger rendEditorWin = RenderingDebugger(String(ICON_FK_VIDEO_CAMERA) + " Rendering");
        Console debugConsole = Console(String(ICON_FK_CHECK_CIRCLE_O) + " Debug Console");
        InputWindow input = InputWindow(String(ICON_FK_KEYBOARD_O) + " Input");
        Inspector inspector = Inspector(String(ICON_FK_SEARCH) + "Inspector");

        // Editor viewport camera
        //CCE::Graphics::Camera viewportCamera = CCE::Graphics::Camera();

#if MULTITHREADED
        //JobWindow jobWin = JobWindow("Jobs");
#endif

        using namespace CCE;

        // frame time measuring
        std::chrono::steady_clock::time_point start{};
        std::chrono::steady_clock::time_point end{};
        
        // update window input
        int rValue = 0;
        // ----------------------------------------

        // @TODO: Multithread the editor loop as well
        while (rValue != (int)WM_QUIT)
        {
            mRuntimeManager.PreEditorUpdate(rValue, true);
            
            //viewportCamera.Update();
            
            // ------------------------------ RUNTIME DEBUGGER ------------------------------

            start = CCE::Time::Now();

            IGUIDrawable::PreGUIUpdate();

            for (int i = 0; i < IGUIDrawable::GetGUIDrawablePtrs()->size(); i++)
            {
                IGUIDrawable::GetGUIDrawablePtrs()->at(i)->UpdateDrawable();
            }
            //ImGui::ShowDemoWindow();
            IGUIDrawable::PostGUIUpdate();
           
            end = CCE::Time::Now(); imgui_process_time_ms = static_cast<float>( CCE::Time::GetDurationInMilliSec( start, end ) );
            // ------------------------------------------------------------------------------

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
