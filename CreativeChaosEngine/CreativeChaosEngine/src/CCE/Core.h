#pragma once

#ifdef CCE_PLATFORM_WINDOWS // PLATFORM WINDOWS

// Windows header
// @TODO: Maybe split up so that its a bit more lightweight
#define NOMINMAX
#include <Windows.h>
#include <windowsx.h>
#include <Lmcons.h>
#include <Shlobj.h>

#ifdef CCE_BUILD_DLL
#define CCE_API __declspec(dllexport)
#else
#define CCE_API __declspec(dllimport)
#endif // CCE_BUILD_DLL
#else
#error CCE is currently only supported for Windows
#endif // CCE_PLATFORM_WINDOWS


// STL header
#include <stdlib.h>
#include <stdio.h>