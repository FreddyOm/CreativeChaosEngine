#pragma once
#include "../Core.h"

#ifdef DEBUG
#define DINFO(msg) CCE::Debug::DebugInfo(msg, __FILE__, __LINE__)
#define DWARNING(msg) CCE::Debug::DebugWarning(msg, __FILE__, __LINE__)
#define DERROR(msg) CCE::Debug::DebugError(msg, __FILE__, __LINE__)
#define DASSERT(condition, msg) CCE::Debug::DebugAssert(condition, msg, __FILE__, __LINE__)
#endif // DEBUG

#ifdef DEBUG_PROFILE
#define DINFO(msg)
#define DWARNING(msg)
#define DERROR(msg) CCE::Debug::DebugError(msg, __FILE__, __LINE__)
#define DASSERT(condition, msg) CCE::Debug::DebugAssert(condition, msg, __FILE__, __LINE__)
#endif // PROFILE

#ifdef NDEBUG
#define DINFO(msg)
#define DWARNING(msg)
#define DERROR(msg)
#define DASSERT(condition, msg)
#endif // RELEASE

namespace CCE
{
	struct CCE_API Debug
	{
		static void DebugInfo(const char* msg, const char* file, const int line) noexcept;
		static void DebugWarning(const char* msg, const char* file, const int line) noexcept;
		static void DebugError(const char* msg, const char* file, const int line) noexcept;
		static void DebugError(const DWORD error, const char* file, const int line) noexcept;
		static void DebugAssert(const bool condition, const char* msg, const char* file, const int line) noexcept;
	private:

	};
}
