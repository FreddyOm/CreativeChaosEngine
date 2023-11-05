#include "UnitTestString.h"
#include "../Logger.h"
#include "../../Analysis/Time.h"
#include "../../String/StringConverter.h"
#include <random>

namespace CCE_Testing
{
	void UnitTestString::Test()
	{
		if (TestString1()) { LOGC_TEST("Testing UnitTestString::TestString1: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestString::TestString1: failed", COLOR_RED); }

		if (TestString2()) { LOGC_TEST("Testing UnitTestString::TestString2: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestString::TestString2: failed", COLOR_RED); }

		if (TestString3()) { LOGC_TEST("Testing UnitTestString::TestString3: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestString::TestString3: failed", COLOR_RED); }

		if (TestString4()) { LOGC_TEST("Testing UnitTestString::TestString4: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestString::TestString4: failed", COLOR_RED); }
	
		if (TestStringRandomized()) { LOGC_TEST("Testing UnitTestString::TestStringRandomized: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestString::TestStringRandomized: failed", COLOR_RED); }

		if (TestStringConversion()) { LOGC_TEST("Testing UnitTestString::TestStringConversion: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestString::TestStringConversion: failed", COLOR_RED); }

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
			&& s3.Length() == 6
			&& s1 != "aHllo" && s1 != "hallo";
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
		CCE::String s4("");
		CCE::String s5(" ");
		CCE::String s6("  ");

		return s1.Length() - 1 == s2.Length()
			&& s1.Length() == 16 && s2.sId != s1.sId
			&& s3.Length() == 10 && s1 == "Mein Test String"
			&& s2 != "Mein Test String" && s2 != "mein Test String"
			&& s4 != s5 && s5 != s6 && s4 != s6;
	}

	bool UnitTestString::TestString4() noexcept
	{
		CCE::String s1("Mein Test String");
		CCE::String s0;
		{
			CCE::String scopeString("Default Test Text");
			s0 = scopeString.Value();
		}

		CCE::String s2("Ein Test String");
		s2 = s1;
		CCE::String s3 = CCE::String("TestString");

		return s1.Length() == s2.Length()
			&& s1 == s2 && s2.sId == s1.sId
			&& s3.Length() == 10 && s3.sId != s1.sId
			&& s3 != s1 && s3 != s2 && s0 == "Default Test Text"
			&& s0.Length() == 17 && s2.sId == 14840547598754946856;
	}

	bool UnitTestString::TestStringRandomized() noexcept
	{
		CCE::String s1 = GetRandomString();
		CCE::String s2 = GetRandomString();

		CCE::String s3 = s1;
		CCE::String s4 = s3;

		bool test1 = s1 == s3;
		bool test2 = s1 == s4;
		bool test3 = s1.sId == s3.sId;
		bool test4 = s4.sId != s2.sId;
		bool test5 = s2 != s3;
		bool test6 = s3.sId == s4.sId;
		bool test7 = strcmp(s3.Value(), s1.Value()) == 0;

		return test1 && test2 && test3 && test4 && test5 && test6 && test7;
	}

	bool UnitTestString::TestStringConversion() noexcept
	{
		std::wstring wstr = std::wstring(L"Test String mit ein paar Zeichen");
		bool test1 = std::string("Test String mit ein paar Zeichen") == 
			StringConverter::WStringToString(wstr);

		wstr = std::wstring(L"Andere Zeichen koennen auch auftauchen... !");
		bool test2 = std::string("Andere Zeichen koennen auch auftauchen... !") ==
			StringConverter::WStringToString(wstr);

		bool test3 = std::wstring(L"Nocheinmal andere Zeichen 938457 ?/.,,:_-<62514354689223+`#+'*") ==
			StringConverter::StringToWString(std::string("Nocheinmal andere Zeichen 938457 ?/.,,:_-<62514354689223+`#+'*"));

		bool test4 = std::wstring(L"^!/&($\"897356()/ <> | -_.:,;12345678*8wefoiuwgh") ==
			StringConverter::StringToWString("^!/&($\"897356()/ <> | -_.:,;12345678*8wefoiuwgh");
	
		bool test5 = std::wstring() == 
			StringConverter::StringToWString(std::string());

		bool test6 = std::string() == 
			StringConverter::WStringToString(std::wstring());

		return test1 && test2 && test3 && test4 && test5 && test6;
	}

	CCE::String UnitTestString::GetRandomString() noexcept
	{
		DWORD64 _seed;
		
		_seed = __rdtsc();
		std::srand(static_cast<unsigned int>(_seed));

		int randomChar = std::rand() % 128;
		int randomStrLen = std::rand() % 256;
		char buf[256];

		for (int i = 0; i < randomStrLen; i++)
		{
			_seed = __rdtsc();
			std::srand((unsigned int)_seed);
			randomChar = std::rand() % 128;

			buf[i] = randomChar;
		}

		buf[randomStrLen] = 0;

		auto str = CCE::String(&buf[0]);

		return str;
	}
}
