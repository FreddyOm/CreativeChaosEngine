#pragma once
#include "../Core.h"

#ifdef DEBUG
#define DINFO(msg) CCE::DebugInfo(msg, __FILE__, __LINE__)
#define DWARNING(msg) CCE::DebugWarning(msg, __FILE__, __LINE__)
#define DERROR(msg) CCE::DebugError(msg, __FILE__, __LINE__)
#define DASSERT(condition, msg) CCE::DebugAssert(condition, msg, __FILE__, __LINE__)
#endif // DEBUG

#ifdef DEBUG_PROFILE
#define DINFO(msg)
#define DWARNING(msg)
#define DERROR(msg) CCE::DebugError(msg, __FILE__, __LINE__)
#define DASSERT(condition, msg) CCE::DebugAssert(condition, msg, __FILE__, __LINE__)
#endif // PROFILE

#ifdef NDEBUG
#define DINFO(msg)
#define DWARNING(msg)
#define DERROR(msg)
#define DASSERT(condition, msg)
#endif // RELEASE

namespace CCE
{
	CCE_API void DebugInfo(const char* msg, const char* file, const int line) noexcept;
	CCE_API void DebugWarning(const char* msg, const char* file, const int line) noexcept;
	CCE_API void DebugError(const char* msg, const char* file, const int line) noexcept;
	CCE_API void DebugError(const DWORD error, const char* file, const int line) noexcept;
	CCE_API void DebugAssert(const bool condition, const char* msg, const char* file, const int line) noexcept;
}
