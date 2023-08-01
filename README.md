# CreativeChaosEditor
A custom game engine editor

## Documentation

### CC Editor

The editor currently acts as a client that uses the engine and its subsystems by calling the DLL.
The **CCEditor** provides a way of initializing the engines **RuntimeManager** and updating the GUI.

#### Editor Window

**EditorWindows** are windows created by ImGui inside the **ClientWindow**.
They can be created by initializing a new EditorWindow before the editor loop starts.
This will register the window and will call it's ***OnGui()*** method.

## TODOs
- [x] Implement ImGui
- [ ] Create new EditorWindows
- [ ] Create Input Debugging Window
- [ ] Change CCEditor implementation to use variants 

# CreativeChaosEngine
Some custom game engine subsystems 

## TODOs
- [x] Implement a central engine loop that runs all the engine calls (as jobs)
- [x] Implement Counters so they move the current job to the wait list.
- [ ] Refactor JobDeclaration and EntryPoint to be more easily used.
- [ ] Add an interface to be able to inject resources into the engine loop (e.g. query assets to renderpipeline) 
- [ ] Refactor custom MemAllocs so they are compatible with stl containers and custom containers
- [ ] Use custom MemAlloc and custom Container everywhere.
- [ ] Differentiate between "Engine Memory" and "Game Memory" (which memory does only the engine need and which will be used in the game)
- [ ] Implement some basic SSE
- [ ] Move testing to individual solution and use it as a dll (?) inside the engine (?)
- [ ] Implement File System
- [ ] Implement Rendering System

## Documentation

### Managers

#### Base Manager

A common class to derive every manager class from. Holds pure virtual functions for ***StartUp()*** and ***ShutDown()***.

#### InputManager

The **InputManager** manages all engine inputs. Currently detected inputs are:

- [x] Mouse and Keyboard
- [x] Up to 4 XInput Devices
- [ ] Up to 4 DualSense Devices (WIP)
- [ ] Up to 4 DirectInput Devices

#### JobManager
The **JobManager** handles the creation, maintenance and destruction of the internal job system.

Jobs can be created by using **JobManager::Declarations**. They get a bound ***JobManager::EntryPoint***, a ***Priority*** 
and the ***Arguments*** for the call.
   
The declarations are then queried by calling **JobManager::KickJob** which will automatically queue the job for execution
and run it whenever the resources are available. To manage race conditions and concurrent resource management use 
**JobManager::WaitForCounter**. This will busy wait for end of execution.
>Currently only ***void*** return types are supported. Values can be handed over as pointers to manipulate local objects. 

#### MemoryManager

The **MemoryManager** holds different custom memory allocators which can be used to allocate memory for subsytem resources.
The memory allocators support ***non-aligned*** and ***aligned*** memory allocation calls.
Use the respective **AllocAligned()** functions for that.

##### Pool Allocator
The pool allocator creates a pool of equally sized memory chunks.

##### Stack Allocator
The stack allocator creates a stack of specified size and can push memory chunks onto it.

#### PhysicsManager

To be created.

#### ProfilingManager

To be created.

#### Runtime Manager
The **RuntimeManager** is the bootstrapping unit on the engine side. It initializes all subsystems in the correct order and provides an interface to 
the engine's update loop.

### Strings
In the CCE, strings are implemented via a StringID. This is a CRC hashed char array which is then saved into
a global string table. When calling a **CCE::String**s **myString.Value()** function, the actual char* is returned.
Adding a new string is therefore not that much more efficent while comparing, reusing or transfering Strings
is significantly improved in terms of performance.

The goal is to create a string system that uses IDs ***everywhere***. In the best case, all strings can be loaded
or written during compiletime and don't need to be created at all after release.

### Graphics

#### Client Window

Create a window by calling the **ClientWindow::OpenWindow(hInstance, "MyWindowName")** function.
After that make sure to call **ClientWindow::UpdateWindow()** to initialize the message handling.
When closing a window use **ClientWindow:CloseEditorWindow()**.

>Multiple windows are currently not supported.

### Utilities

Utilities currently include:

- [x] Custom color description and parser
- [x] Spin locks for use by the job system
- [x] Custom math functions (e.g. 64 bit CRC-Hash)
- [ ] Custom event wrapper
