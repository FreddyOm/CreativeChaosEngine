#include "UnitTestString.h"
#include "../Logger.h"

namespace CCE_Testing
{
	void UnitTestString::Test()
	{
		if (TestString1()) { LOGC("[TEST] Testing UnitTestString::TestString1: successful", COLOR_GREEN); }
		else { LOGC("[TEST] Testing UnitTestString::TestString1: failed", COLOR_RED); }

		if (TestString2()) { LOGC("[TEST] Testing UnitTestString::TestString2: successful", COLOR_GREEN); }
		else { LOGC("[TEST] Testing UnitTestString::TestString2: failed", COLOR_RED); }

		if (TestString3()) { LOGC("[TEST] Testing UnitTestString::TestString3: successful", COLOR_GREEN); }
		else { LOGC("[TEST] Testing UnitTestString::TestString3: failed", COLOR_RED); }

		if (TestString4()) { LOGC("[TEST] Testing UnitTestString::TestString4: successful", COLOR_GREEN); }
		else { LOGC("[TEST] Testing UnitTestString::TestString4: failed", COLOR_RED); }

	}

	void UnitTestString::Cleanup()
	{

	}

	bool UnitTestString::TestString1() noexcept
	{
		CCE::String s1("Hallo");
		CCE::String s2 = CCE::String("Hallo");
		CCE::String s3 = CCE::String("Hallo ");

		return s1 == s2 && s2 != s3 
			&& s1.Length() == 5 
			&& s3.Length() == 6;
	}

	bool UnitTestString::TestString2() noexcept
	{
		CCE::String s1("TestString der ein bisschen länger ist");
		CCE::String s2(s1.Value());
		CCE::String s3 = s2.Value();
		CCE::String s4 = CCE::String("TestString");

		return s1 == s2 && s2 == s3
			&& s2.Length() == s3.Length()
			&& strcmp(s4.Value(), "TestString") == 0;
	}

	bool UnitTestString::TestString3() noexcept
	{
		CCE::String s1("Mein Test String");
		CCE::String s2("Ein Test String");
		CCE::String s3 = CCE::String("TestString");

		return s1.Length() - 1 == s2.Length()
			&& s1.Length() == 16 && s2.sId != s1.sId
			&& s3.Length() == 10;
	}

	bool UnitTestString::TestString4() noexcept
	{
		CCE::String s1("Mein Test String");
		CCE::String s2("Ein Test String");
		s2 = s1;
		CCE::String s3 = CCE::String("TestString");

		return s1.Length() == s2.Length()
			&& s1 == s2 && s2.sId == s1.sId
			&& s3.Length() == 10 && s3.sId != s1.sId
			&& s3 != s1 && s3 != s2;
	}
}
