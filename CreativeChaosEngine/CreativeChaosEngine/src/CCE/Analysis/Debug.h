#pragma once
#include "../Core.h"

namespace CCE
{
	struct CCE_API Debug
	{
		static void DebugInfo(const char* msg, const char* file, const int line) noexcept;
		static void DebugWarning(const char* msg, const char* file, const int line) noexcept;
		static void DebugError(const char* msg, const char* file, const int line) noexcept;
		static void DebugAssert(bool condition, const char* msg, const char* file, const int line) noexcept;

	private:
		static const char* infoTag;
		static const char* warningTag;
		static const char* errorTag;
		static const char* assertTag;
	};
}
