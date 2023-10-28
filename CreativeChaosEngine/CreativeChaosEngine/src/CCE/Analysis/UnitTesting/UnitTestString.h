#pragma once
#include "UnitTest.h"
#include "../../String/String.h"

namespace CCE_Testing
{
	struct CCE_API UnitTestString : public UnitTest
	{
		friend struct CCE::String;

		void Test() override;
		void Cleanup() override;

	private:
		bool TestString1() noexcept;
		bool TestString2() noexcept;
		bool TestString3() noexcept;
		bool TestString4() noexcept;

		bool TestStringRandomized() noexcept;
		bool TestStringConversion() noexcept;
	
		CCE::String GetRandomString() noexcept;
	};
}



