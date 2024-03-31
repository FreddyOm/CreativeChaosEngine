#pragma once
#include "../../core.h"

namespace CCE_Testing
{
	struct CCE_API UnitTest
	{
		UnitTest() = default;
		~UnitTest() = default;

		virtual void Test() = 0;
		virtual void Cleanup()
		{

		}
	};
}
