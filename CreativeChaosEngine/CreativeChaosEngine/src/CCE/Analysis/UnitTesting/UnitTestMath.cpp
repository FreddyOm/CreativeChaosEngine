#include "UnitTestMath.h"
#include "../Logger.h"
#include <cmath>

namespace CCE_Testing
{
	void UnitTestMath::Test()
	{
		if (TestAdd1()) { LOGC_TEST("Testing CCMath::Add1: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing CCMath::Add1: failed", COLOR_RED); }

		if (TestAdd2()) { LOGC_TEST("Testing CCMath::Add2: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing CCMath::Add2: failed", COLOR_RED); }

		if (TestAdd3()) { LOGC_TEST("Testing CCMath::Add3: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing CCMath::Add3: failed", COLOR_RED); }

		if (TestAdd4()) { LOGC_TEST("Testing CCMath::Add4: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing CCMath::Add4: failed", COLOR_RED); }

		if (TestAdd5()) { LOGC_TEST("Testing CCMath::Add5: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing CCMath::Add5: failed", COLOR_RED); }


		if(TestSub1()) { LOGC_TEST("Testing CCMath::Sub1: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing CCMath::Sub1: failed", COLOR_RED); }

		if (TestSub2()) { LOGC_TEST("Testing CCMath::Sub2: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing CCMath::Sub2: failed", COLOR_RED); }

		if (TestSub3()) { LOGC_TEST("Testing CCMath::Sub3: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing CCMath::Sub3: failed", COLOR_RED); }

		if (TestSub4()) { LOGC_TEST("Testing CCMath::Sub4: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing CCMath::Sub4: failed", COLOR_RED); }

		if (TestSub5()) { LOGC_TEST("Testing CCMath::Sub5: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing CCMath::Sub5: failed", COLOR_RED); }
	}

	void UnitTestMath::Cleanup()
	{}

	bool UnitTestMath::TestAdd1() noexcept
	{
		int a = 7;
		int b = 9;

		int res;

		CCE::Math::CCMath::Add(&a, &b, &res);

		if (res == 16)
		{
			return true;
		}

		return false;
	}

	bool UnitTestMath::TestAdd2() noexcept
	{
		int a = -800;
		int b = 400;

		int res;

		CCE::Math::CCMath::Add(&a, &b, &res);

		if (res == -400)
		{
			return true;
		}

		return false;
	}

	bool UnitTestMath::TestAdd3() noexcept
	{
		float a = -1.4f;
		float b = 4.4f;

		float res;

		CCE::Math::CCMath::Add(&a, &b, &res);

		if (res == 3.0f)
		{
			return true;
		}
		return false;
	}

	bool UnitTestMath::TestAdd4() noexcept
	{
		long a = 50000000;
		long b = 12000000;

		long res;

		CCE::Math::CCMath::Add(&a, &b, &res);

		if (res == 62000000)
		{
			return true;
		}
		return false;
	}

	bool UnitTestMath::TestAdd5() noexcept
	{
		double a = 7.9;
		double b = -8.0;

		double res;

		CCE::Math::CCMath::Add(&a, &b, &res);

		// FIXME: Double uncertainty!! Fix in CCE::Math::CCMath::Add
		if (res == 0.1)
		{
			return true;
		}
		return false;
	}


	bool UnitTestMath::TestSub1() noexcept
	{
		int a = 7;
		int b = 9;

		int res;

		CCE::Math::CCMath::Substract(&a, &b, &res);

		if (res == -2)
		{
			return true;
		}
		return false;
	}

	bool UnitTestMath::TestSub2() noexcept
	{
		int a = -800;
		int b = 400;

		int res;

		CCE::Math::CCMath::Substract(&a, &b, &res);

		if (res == -1200)
		{
			return true;
		}
		return false;
	}

	bool UnitTestMath::TestSub3() noexcept
	{
		float a = -1.4f;
		float b = 4.4f;

		float res;

		CCE::Math::CCMath::Substract(&a, &b, &res);

		if (res == -5.8f)
		{
			return true;
		}
		return false;
	}

	bool UnitTestMath::TestSub4() noexcept
	{
		long a = 50000000;
		long b = 12000000;

		long res;

		CCE::Math::CCMath::Substract(&a, &b, &res);

		if (res == 38000000)
		{
			return true;
		}
		return false;
	}
	
	bool UnitTestMath::TestSub5() noexcept
	{
		double a = 7.9;
		double b = -8.0;

		double res;

		CCE::Math::CCMath::Substract(&a, &b, &res);

		if (res == 15.9)
		{
			return true;
		}
		return false;
	}
}