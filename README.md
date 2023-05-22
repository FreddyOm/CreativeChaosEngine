# CreativeChaosEditor
The custom game engine editor of 'CreativeChaos UG'

***THIS DOCUMENT IS NOT MEANT FOR PUBLIC DISPLAY!***

## Documentation

## TODOs
- Integrate and finalize ImGui

# CreativeChaosEngine
The custom game engine of 'CreativeChaos UG'

***THIS DOCUMENT IS NOT MEANT FOR PUBLIC DISPLAY!***

## TODOs
- Implement Counters so they move the current job to the wait list.
- Refactor JobDeclaration and EntryPoint to be more easily used.
- Use custom MemAlloc and custom Container everywhere.

## Documentation

### JobSystem
Jobs can be created by using **JobManager::Declarations**. They get a bound ***JobManager::EntryPoint***, a ***Priority*** 
and the ***Arguments*** for the call.
>Currently only ***void*** return types are supported. Values can be handed over as pointers to manipulate local objects.    
The declarations are then queried by calling **JobManager::KickJob** which will automatically queue the job for execution
and run it whenever the resources are available. To manage race conditions and concurrent resource management use 
**JobManager::WaitForCounter**. This will busy wait for end of execution.

### Memory Allocators
The Memory Allocators sopport ***non-aligned*** and ***aligned*** memory allocation calls.
Use the respective **AllocAligned()** functions for that.

#### Pool Allocator
The pool allocator creates a pool of equally sized memory chunks.

#### Stack Allocator
The stack allocator creates a stack of specified size and can push memory chunks onto it.

### Strings
In the CCE, strings are implemented via a StringID. This is a CRC hashed char array which is then saved into
a global string table. When calling a **CCE::String**s **myString.Value()** function, the actual char* is returned.
Adding a new string is therefore not that much more efficent while comparing, reusing or transfering Strings
is significantly improved in terms of performance.

The goal is to create a string system that uses IDs ***everywhere***. In the best case, all strings can be loaded
or written during compiletime and don't need to be created at all after release.

### Window
Create a window by calling the **EditorWindow::OpenWindow(hInstance, "MyWindowName")** function.
After that make sure to call **EditorWindow::UpdateWindow()** to initialize the message handling.
When closing a window use **EditorWindow:CloseEditorWindow()**.

Multiple windows are currently not supported.
