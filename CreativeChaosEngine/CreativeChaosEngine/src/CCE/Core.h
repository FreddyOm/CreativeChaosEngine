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


namespace CCE
{
	class CCE_API Core
	{
	public:
		Core() = default;
		~Core() = default;
		void Test();
	};
}
