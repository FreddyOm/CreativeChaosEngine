#pragma once
#include "UnitTest.h"
#include "../../Utilities/CCMath.h"

namespace CCE_Testing
{
	struct CCE_API UnitTestMath : public UnitTest
	{
		friend class CCMath;

		void Test() override;
		void Cleanup() override;

	private:
		bool TestAdd1() noexcept;
		bool TestAdd2() noexcept;
		bool TestAdd3() noexcept;
		bool TestAdd4() noexcept;
		bool TestAdd5() noexcept;

		bool TestSub1() noexcept;
		bool TestSub2() noexcept;
		bool TestSub3() noexcept;
		bool TestSub4() noexcept;
		bool TestSub5() noexcept;

	};
}
