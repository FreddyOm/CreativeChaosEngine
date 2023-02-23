# CreativeChaosEngine
The custom game engine of 'CreativeChaos UG'

***THIS DOCUMENT IS NOT MEANT FOR PUBLIC DISPLAY!***


## Documentation


### Window

Create a window by calling the **EditorWindow::OpenWindow(hInstance, "MyWindowName")** function.
After that make sure to call **EditorWindow::UpdateWindow()** to initialize the message handling.

Multiple windows are currently not supported.

### Strings

In the CCE, strings are implemented via a StringID. This is a CRC hashed char array which is then saved into
a global string table. When calling a **CCE::String**s **myString.Value()** function, the actual char* is returned.
Adding a new string is therefore not that much more efficent while comparing, reusing or transfering Strings
is significantly improved in terms of performance.

The goal is to create a string system that uses IDs ***everywhere***. In the best case, all strings can be loaded
or written during compiletime and don't need to be created at all after release.

>**To fix:** Empty strings and strings containing only spaces create an assertion because of a hash collision.
