#include "UnitTestStackAlloc.h"
#include "../Logger.h"

namespace CCE_Testing
{
	void UnitTestStackAlloc::Test()
	{
		if (TestAlloc1()) { LOGC_TEST("Testing UnitTestStackAlloc::TestAlloc1: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestStackAlloc::TestAlloc1: failed", COLOR_RED); }

		if (TestAlloc2()) { LOGC_TEST("Testing UnitTestStackAlloc::TestAlloc2: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestStackAlloc::TestAlloc2: failed", COLOR_RED); }

		if (TestAlloc3()) { LOGC_TEST("Testing UnitTestStackAlloc::TestAlloc3: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestStackAlloc::TestAlloc3: failed", COLOR_RED); }

		if (TestAlloc4()) { LOGC_TEST("Testing UnitTestStackAlloc::TestAlloc4: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestStackAlloc::TestAlloc4: failed", COLOR_RED); }


		if (TestFree1()) { LOGC_TEST("Testing UnitTestStackAlloc::TestFree1: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestStackAlloc::TestFree1: failed", COLOR_RED); }

		if (TestFree2()) { LOGC_TEST("Testing UnitTestStackAlloc::TestFree2: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestStackAlloc::TestFree2: failed", COLOR_RED); }

		if (TestFree3()) { LOGC_TEST("Testing UnitTestStackAlloc::TestFree3: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestStackAlloc::TestFree3: failed", COLOR_RED); }

		if (TestFree4()) { LOGC_TEST("Testing UnitTestStackAlloc::TestFree4: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestStackAlloc::TestFree4: failed", COLOR_RED); }


		if (TestClear1()) { LOGC_TEST("Testing UnitTestStackAlloc::TestClear1: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestStackAlloc::TestClear1: failed", COLOR_RED); }

		if (TestClear2()) { LOGC_TEST("Testing UnitTestStackAlloc::TestClear2: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestStackAlloc::TestClear2: failed", COLOR_RED); }

		if (TestClear3()) { LOGC_TEST("Testing UnitTestStackAlloc::TestClear3: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestStackAlloc::TestClear3: failed", COLOR_RED); }

		if (TestClear4()) { LOGC_TEST("Testing UnitTestStackAlloc::TestClear4: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestStackAlloc::TestClear4: failed", COLOR_RED); }

	
		if (TestMarker1()) { LOGC_TEST("Testing UnitTestStackAlloc::TestMarker1: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestStackAlloc::TestMarker1: failed", COLOR_RED); }
	
		if (TestMarker2()) { LOGC_TEST("Testing UnitTestStackAlloc::TestMarker2: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestStackAlloc::TestMarker2: failed", COLOR_RED); }

		if (TestMarker3()) { LOGC_TEST("Testing UnitTestStackAlloc::TestMarker3: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestStackAlloc::TestMarker3: failed", COLOR_RED); }

		if (TestMarker4()) { LOGC_TEST("Testing UnitTestStackAlloc::TestMarker4: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestStackAlloc::TestMarker4: failed", COLOR_RED); }


		if (TestGetTotalMem()) { LOGC_TEST("Testing UnitTestStackAlloc::TestGetTotalMem: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestStackAlloc::TestGetTotalMem: failed", COLOR_RED); }

		if (TestGetUsedMem()) { LOGC_TEST("Testing UnitTestStackAlloc::TestGetUsedMem: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestStackAlloc::TestGetUsedMem: failed", COLOR_RED); }

		if (TestGetFreeMem()) { LOGC_TEST("Testing UnitTestStackAlloc::TestGetFreeMem: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestStackAlloc::TestGetFreeMem: failed", COLOR_RED); }
		
		if (TestGetNumAllocs()) { LOGC_TEST("Testing UnitTestStackAlloc::TestGetNumAllocs: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestStackAlloc::TestGetNumAllocs: failed", COLOR_RED); }

		if (TestGetNumFrees()) { LOGC_TEST("Testing UnitTestStackAlloc::TestGetNumFrees: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestStackAlloc::TestGetNumFrees: failed", COLOR_RED); }

		if (TestGetCurrentTop()) { LOGC_TEST("Testing UnitTestStackAlloc::TestGetCurrentTop: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestStackAlloc::TestGetCurrentTop: failed", COLOR_RED); }
	

		if (TestAlignedAlloc1()) { LOGC_TEST("Testing UnitTestStackAlloc::TestAlignedAlloc1: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestStackAlloc::TestAlignedAlloc1: failed", COLOR_RED); }
	
		if (TestAlignedAlloc2()) { LOGC_TEST("Testing UnitTestStackAlloc::TestAlignedAlloc2: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestStackAlloc::TestAlignedAlloc2: failed", COLOR_RED); }

		if (TestAlignedFree1()) { LOGC_TEST("Testing UnitTestStackAlloc::TestAlignedFree1: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestStackAlloc::TestAlignedFree1: failed", COLOR_RED); }

		if (TestAlignedFree2()) { LOGC_TEST("Testing UnitTestStackAlloc::TestAlignedFree2: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestStackAlloc::TestAlignedFree2: failed", COLOR_RED); }
	}

	void UnitTestStackAlloc::Cleanup()
	{
		alloc1.~StackAllocator();
		alloc2.~StackAllocator();
		alloc3.~StackAllocator();
		alloc4.~StackAllocator();
		alloc5.~StackAllocator();
		alloc6.~StackAllocator();
	}

	bool UnitTestStackAlloc::TestAlloc1() noexcept
	{
		TestStruct1* ts = alloc1.Alloc<TestStruct1>();
		bool check1 = alloc1.GetNumAllocs() == 1;
		bool check2 = alloc1.GetUsedMem() == 8;
		
		return check1 && check2;
	}

	bool UnitTestStackAlloc::TestAlloc2() noexcept
	{
		TestStruct1* ts = alloc1.Alloc<TestStruct1>();
		bool check1 = ts != nullptr;
		bool check2 = alloc1.GetUsedMem() == 16;
		bool check3 = alloc1.GetFreeMem() == 1008;

		return check1 && check2 && check3;
	}

	bool UnitTestStackAlloc::TestAlloc3() noexcept
	{
		TestStruct2* ts = alloc1.Alloc<TestStruct2>();
		bool check1 = ts != nullptr;
		bool check2 = alloc1.GetUsedMem() == 48;
		bool check3 = alloc1.GetFreeMem() == 976;
		bool check4 = false;
		if (ts != nullptr)
		{
			ts->a = 0.2f;
			check4 = ts->a == 0.2f;
		}
			
		return check1 && check2 && check3;
	}

	bool UnitTestStackAlloc::TestAlloc4() noexcept
	{
		TestStruct1* ts = alloc1.Alloc<TestStruct1>();
		bool check1 = ts != nullptr;
		bool check2 = false;
		if (ts != nullptr)
		{
			ts->a = 'a';
			check2 = ts->a == 'a';
		}		

		return check1 && check2;
	}

	bool UnitTestStackAlloc::TestFree1() noexcept
	{
		auto* ts = alloc2.Alloc<TestStruct2>();
		bool check1 = alloc2.GetNumAllocs() == 1;
		bool check2 = alloc2.GetUsedMem() == 32;
		alloc2.Free(sizeof(TestStruct2));
		bool check3 = alloc2.GetNumFrees() == 1;
		bool check4 = alloc2.GetUsedMem() == 0;

		return check1 && check2 && check3 && check4;
	}

	bool UnitTestStackAlloc::TestFree2() noexcept
	{
		auto* ts = alloc3.Alloc<TestStruct2>();
		bool check1 = alloc3.GetUsedMem() == 32;
		auto* ts1 = alloc3.Alloc<TestStruct2>();
		bool check2 = alloc3.GetUsedMem() == 64;
		bool check3 = alloc3.GetNumAllocs() == 2;

		alloc3.Free(sizeof(TestStruct2));
		bool check4 = alloc3.GetNumFrees() == 1;
		bool check5 = alloc3.GetUsedMem() == 32;

		return check1 && check2 && check3 && check4 && check5;

		return false;
	}

	bool UnitTestStackAlloc::TestFree3() noexcept
	{
		alloc3.Free(sizeof(TestStruct2));
		bool check1 = alloc3.GetNumFrees() == 2;
		bool check2 = alloc3.GetUsedMem() == 0;

		alloc3.Free(sizeof(TestStruct2));
		bool check3 = alloc3.GetNumFrees() == 2;
		bool check4 = alloc3.GetUsedMem() == 0;

		return check1 && check2 && check3 && check4;
	}

	bool UnitTestStackAlloc::TestFree4() noexcept
	{
		alloc2.Alloc<TestStruct2>();

		alloc2.Free(sizeof(TestStruct1));
		alloc2.Free(sizeof(TestStruct1));
		alloc2.Free(sizeof(TestStruct1));
		alloc2.Free(sizeof(TestStruct1));

		bool check1 = alloc2.GetNumFrees() == 5;
		bool check2 = alloc2.GetUsedMem() == 0;
		bool check3 = alloc2.GetTotalMem() == 32;

		return check1 && check2 && check3;
	}

	bool UnitTestStackAlloc::TestClear1() noexcept
	{
		alloc1.ClearAll();
		bool check1 = alloc1.GetFreeMem() == 1024;
		bool check2 = alloc1.GetUsedMem() == 0;
		bool check3 = alloc1.GetUsedSpaceRatio() == 0.0f;
		bool check4 = alloc1.GetTotalMem() == 1024;

		return check1 && check2 && check3 && check4;
	}

	bool UnitTestStackAlloc::TestClear2() noexcept
	{
		TestStruct2* ts = alloc2.Alloc<TestStruct2>();
		bool check1 = alloc2.GetUsedMem() == 32;
		bool check2 = alloc2.GetFreeMem() == 0;
		alloc2.ClearAll();
		bool check3 = alloc2.GetUsedMem() == 0;
		bool check4 = alloc2.GetFreeMem() == 32;


		return check1 && check2 && check3 && check4;
	}

	bool UnitTestStackAlloc::TestClear3() noexcept
	{
		auto* ts = alloc3.Alloc<TestStruct3>();

		bool check1 = alloc3.GetUsedMem() >= 128;
		bool check2 = alloc3.GetTotalMem() == 2048000;
		bool check3 = alloc3.GetFreeMem() == alloc3.GetTotalMem() - alloc3.GetUsedMem();

		alloc3.ClearAll();

		bool check4 = alloc3.GetFreeMem() == alloc3.GetTotalMem();
		bool check5 = alloc3.GetUsedMem() == 0;

		return check1 && check2 && check3 && check4 && check5;
	}

	bool UnitTestStackAlloc::TestClear4() noexcept
	{
		alloc4.Alloc<TestStruct2>();
		bool check1 = alloc4.GetTotalMem() == 8192;
		bool check2 = alloc4.GetUsedMem() == 32;

		alloc4.ClearAll();
		alloc4.ClearAll();

		bool check3 = alloc4.GetUsedMem() == 0;
		bool check4 = alloc4.GetFreeMem() == 8192;

		return check1 && check2 && check3 && check4;
	}

	bool UnitTestStackAlloc::TestMarker1() noexcept
	{
		alloc1.Alloc<TestStruct2>();
		CCMemory::StackAllocMarker marker = 
			(intptr_t) alloc1.Alloc<TestStruct3>();
		alloc1.Alloc<TestStruct2>();
		alloc1.Alloc<TestStruct2>();

		bool check1 = alloc1.GetUsedMem() == 224;

		alloc1.RollbackToMarker(marker);

		bool check2 = alloc1.GetUsedMem() == 32;

		return check1 && check2;
	}

	bool UnitTestStackAlloc::TestMarker2() noexcept
	{
		alloc2.ClearAll();

		alloc2.Alloc<TestStruct1>();
		CCMemory::StackAllocMarker marker =
			(intptr_t)alloc2.Alloc<TestStruct1>();
		alloc2.Alloc<TestStruct1>();

		alloc2.RollbackToMarker(marker);

		return alloc2.GetUsedMem() == 8;
	}

	bool UnitTestStackAlloc::TestMarker3() noexcept
	{
		alloc1.ClearAll();
		
		CCMemory::StackAllocMarker marker = (intptr_t) alloc1.Alloc<TestStruct3>();
		alloc1.Alloc<TestStruct3>();

		CCMemory::StackAllocMarker marker2 = (intptr_t) alloc1.Alloc<TestStruct3>();
		alloc1.Alloc<TestStruct3>();
		CCMemory::StackAllocMarker marker3 = (intptr_t) alloc1.Alloc<TestStruct3>();

		bool check1 = alloc1.GetUsedMem() == ((intptr_t)5 * 128);

		alloc1.RollbackToMarker(marker2);

		bool check2 = alloc1.GetUsedMem() == ((intptr_t)2 * 128);

		alloc1.RollbackToMarker(marker3);

		bool check3 = alloc1.GetUsedMem() == ((intptr_t)2 * 128);

		alloc1.RollbackToMarker(marker);

		bool check4 = alloc1.GetUsedMem() == 0;

		return check1 && check2 && check3 && check4;
	}

	bool UnitTestStackAlloc::TestMarker4() noexcept
	{
		alloc4.ClearAll();
		alloc4.Alloc<TestStruct3>();
		
		CCMemory::StackAllocMarker marker = alloc4.GetCurretTop();

		alloc4.Alloc<TestStruct3>();
		alloc4.Alloc<TestStruct3>();

		bool check1 = alloc4.GetUsedMem() == ((intptr_t)3 * 128);

		alloc4.RollbackToMarker(marker);

		bool check2 = alloc4.GetUsedMem() == 128;

		return check1 && check2;
	}

	bool UnitTestStackAlloc::TestGetTotalMem() noexcept
	{
		alloc4.ClearAll();
		
		alloc4.Alloc<TestStruct1>();
		alloc4.Alloc<TestStruct1>();
		alloc4.Alloc<TestStruct1>();
		alloc4.Alloc<TestStruct1>();
		
		alloc4.Free(sizeof(TestStruct1));
		alloc4.Free(sizeof(TestStruct1));
		alloc4.Free(sizeof(TestStruct1));
		alloc4.Free(sizeof(TestStruct1));
		
		return alloc4.GetTotalMem() == 8192;
	}

	bool UnitTestStackAlloc::TestGetUsedMem() noexcept
	{
		alloc3.ClearAll();
		alloc3.Alloc<TestStruct3>();
		alloc3.Alloc<TestStruct3>();

		return alloc3.GetUsedMem() == 256;
	}

	bool UnitTestStackAlloc::TestGetFreeMem() noexcept
	{
		alloc1.ClearAll();
		alloc1.Alloc<TestStruct3>();
		alloc1.Alloc<TestStruct3>();

		return alloc1.GetFreeMem() == ((intptr_t)1024 - 256);
	}

	bool UnitTestStackAlloc::TestGetNumAllocs() noexcept
	{
		CCMemory::StackAllocator sa = CCMemory::StackAllocator(128);
		auto* frst = sa.Alloc<TestStruct1>();
		auto* scnd = sa.Alloc<TestStruct1>();
		auto* thrd = sa.Alloc<TestStruct1>();

		CCMemory::StackAllocator sa2 = CCMemory::StackAllocator(16);
		auto* _frst = sa2.Alloc<TestStruct1>();
		auto* _scnd = sa2.Alloc<TestStruct1>();
		auto* _thrd = sa2.Alloc<TestStruct1>();

		return sa.GetNumAllocs() == 3 && sa2.GetNumAllocs() == 2;
	}

	bool UnitTestStackAlloc::TestGetNumFrees() noexcept
	{
		CCMemory::StackAllocator sa = CCMemory::StackAllocator(128);
		auto* frst = sa.Alloc<TestStruct1>();
		auto* scnd = sa.Alloc<TestStruct1>();
		auto* thrd = sa.Alloc<TestStruct1>();

		sa.Free(sizeof(TestStruct1));
		sa.Free(sizeof(TestStruct1));
		sa.Free(sizeof(TestStruct1));
		sa.Free(sizeof(TestStruct1));

		return sa.GetNumFrees() == 3;
	}

	bool UnitTestStackAlloc::TestGetCurrentTop() noexcept
	{
		CCMemory::StackAllocator sa = CCMemory::StackAllocator(128);
		auto* frst = sa.Alloc<TestStruct1>();
		auto* scnd = sa.Alloc<TestStruct1>();
		bool check1 = sa.GetCurretTop() == (intptr_t)frst + 16;
		auto* thrd = sa.Alloc<TestStruct1>();

		sa.Free(sizeof(TestStruct1));
		sa.Free(sizeof(TestStruct1));
		bool check2 = sa.GetCurretTop() == (intptr_t)frst + 8;
		sa.Free(sizeof(TestStruct1));
		sa.Free(sizeof(TestStruct1));

		return check1 && check2;
	}

	bool UnitTestStackAlloc::TestAlignedAlloc1() noexcept
	{
		TestStruct1* ts = alloc5.AllocAligned<TestStruct1>();

		bool check1 = (intptr_t)ts % sizeof(TestStruct1) == 0;
		ts->a = 'b';
		ts->b = ts->a;
		bool check2 = ts->b == 'b';
		
		TestStruct2* ts2 = alloc5.AllocAligned<TestStruct2>();
		bool check3 = (intptr_t)ts2 % sizeof(TestStruct2) == 0;

		return check1 && check2 && check3;
	}

	bool UnitTestStackAlloc::TestAlignedAlloc2() noexcept
	{
		TestStruct1* ts = alloc6.AllocAligned<TestStruct1>();
		bool check1 = ts == nullptr;

		TestStruct1* ts2 = alloc5.AllocAligned<TestStruct1>();
		bool check2 = ts2 != nullptr;

		bool check3 = alloc6.GetNumAllocs() == 0;

		return check1 && check2 && check3;
	}

	bool UnitTestStackAlloc::TestAlignedFree1() noexcept
	{
		auto top = alloc5.GetCurretTop();
		alloc5.FreeAligned(sizeof(TestStruct1));
		auto top2 = alloc5.GetCurretTop();
		bool check1 = top2 < top;

		bool check2 = alloc5.GetNumAllocs() == 3;
		bool check3 = alloc5.GetNumFrees() == 1;

		return check1 && check2 && check3;
	}

	bool UnitTestStackAlloc::TestAlignedFree2() noexcept
	{
		auto top = alloc6.GetCurretTop();
		alloc6.FreeAligned(sizeof(TestStruct1));
		auto top2 = alloc6.GetCurretTop();

		bool check1 = top == top2;
		bool check2 = alloc6.GetNumFrees() == 0;

		auto* p1 = alloc6.AllocAligned<char>();

		bool check3 = p1 != nullptr;
		bool check4 = (intptr_t)p1 > top;
		bool check5 = alloc6.GetNumAllocs() == 1;

		alloc6.FreeAligned(sizeof(char));

		bool check6 = alloc6.GetCurretTop() == top;
		bool check7 = alloc6.GetNumFrees() == 1;

		return check1 && check2 && check3 && check4 
			&& check5 && check6 && check7;
	}
}