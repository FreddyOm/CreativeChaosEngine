#pragma once

#ifdef CCE_PLATFORM_WINDOWS

#ifdef CCE_BUILD_DLL
#define CCE_API __declspec(dllexport)
#else
#define CCE_API __declspec(dllimport)
#endif // CCE_BUILD_DLL
#else
#error CCE is currently only supported for Windows
#endif // CCE_PLATFORM_WINDOWS

// ----------- MACROS ------------

#ifdef DEBUG
#include "Analysis/Debug.h"
#include "Analysis/Logger.h"

#define LOG(msg) CCE::Logger::Log(msg, COLOR_WHITE)
#define LOGC(msg, c) CCE::Logger::Log(msg, c)

#define DINFO(msg) CCE::Debug::DebugInfo(msg, __FILE__, __LINE__)
#define DWARNING(msg) CCE::Debug::DebugWarning(msg, __FILE__, __LINE__)
#define DERROR(msg) CCE::Debug::DebugError(msg, __FILE__, __LINE__)
#define DASSERT(condition, msg) CCE::Debug::DebugAssert(condition, msg, __FILE__, __LINE__)
#endif // DEBUG

#ifdef DEBUG_PROFILE
#include "Analysis/Debug.h"
#include "Analysis/Logger.h"
#define LOG(msg) CCE::Logger::Log(msg)
#define LOGC(msg, c) CCE::Logger::Log(msg, c)

#define DINFO(msg)
#define DWARNING(msg)
#define DERROR(msg) CCE::Debug::DebugError(msg, __FILE__, __LINE__)
#define DASSERT(condition, msg) CCE::Debug::DebugAssert(condition, msg, __FILE__, __LINE__)
#endif // DEBUG_PROFILE

#ifdef NDEBUG
#define LOG(msg)
#define LOGC(msg, c)

#define DINFO(msg)
#define DWARNING(msg)
#define DERROR(msg)
#define DASSERT(condition, msg)
#endif // RELEASE

// -------------------------------
