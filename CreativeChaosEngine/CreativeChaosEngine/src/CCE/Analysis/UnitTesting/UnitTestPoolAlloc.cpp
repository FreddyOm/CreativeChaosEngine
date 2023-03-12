#include "UnitTestPoolAlloc.h"
#include "../Logger.h"

namespace CCE_Testing
{
	void UnitTestPoolAlloc::Test()
	{
		if (TestAlloc1()) { LOGC_TEST("Testing UnitTestPoolAlloc::TestAlloc1: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestPoolAlloc::TestAlloc1: failed", COLOR_RED); }

		if (TestAlloc2()) { LOGC_TEST("Testing UnitTestPoolAlloc::TestAlloc2: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestPoolAlloc::TestAlloc2: failed", COLOR_RED); }

		if (TestAlloc3()) { LOGC_TEST("Testing UnitTestPoolAlloc::TestAlloc3: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestPoolAlloc::TestAlloc3: failed", COLOR_RED); }

		if (TestAlloc4()) { LOGC_TEST("Testing UnitTestPoolAlloc::TestAlloc4: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestPoolAlloc::TestAlloc4: failed", COLOR_RED); }


		if (TestFree1()) { LOGC_TEST("Testing UnitTestPoolAlloc::TestFree1: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestPoolAlloc::TestFree1: failed", COLOR_RED); }

		if (TestFree2()) { LOGC_TEST("Testing UnitTestPoolAlloc::TestFree2: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestPoolAlloc::TestFree2: failed", COLOR_RED); }

		if (TestFree3()) { LOGC_TEST("Testing UnitTestPoolAlloc::TestFree3: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestPoolAlloc::TestFree3: failed", COLOR_RED); }

		if (TestFree4()) { LOGC_TEST("Testing UnitTestPoolAlloc::TestFree4: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestPoolAlloc::TestFree4: failed", COLOR_RED); }


		if (TestClear1()) { LOGC_TEST("Testing UnitTestPoolAlloc::TestClear1: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestPoolAlloc::TestClear1: failed", COLOR_RED); }

		if (TestClear2()) { LOGC_TEST("Testing UnitTestPoolAlloc::TestClear2: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestPoolAlloc::TestClear2: failed", COLOR_RED); }

		if (TestClear3()) { LOGC_TEST("Testing UnitTestPoolAlloc::TestClear3: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestPoolAlloc::TestClear3: failed", COLOR_RED); }

		if (TestClear4()) { LOGC_TEST("Testing UnitTestPoolAlloc::TestClear4: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestPoolAlloc::TestClear4: failed", COLOR_RED); }


		if (TestGetTotalMem()) { LOGC_TEST("Testing UnitTestPoolAlloc::TestGetTotalMem: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestPoolAlloc::TestGetTotalMem: failed", COLOR_RED); }

		if (TestGetUsedMem()) { LOGC_TEST("Testing UnitTestPoolAlloc::TestGetUsedMem: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestPoolAlloc::TestGetUsedMem: failed", COLOR_RED); }

		if (TestGetFreeMem()) { LOGC_TEST("Testing UnitTestPoolAlloc::TestGetFreeMem: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestPoolAlloc::TestGetFreeMem: failed", COLOR_RED); }

		if (TestGetNumAllocs()) { LOGC_TEST("Testing UnitTestPoolAlloc::TestGetNumAllocs: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestPoolAlloc::TestGetNumAllocs: failed", COLOR_RED); }

		if (TestGetNumFrees()) { LOGC_TEST("Testing UnitTestPoolAlloc::TestGetNumFrees: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestPoolAlloc::TestGetNumFrees: failed", COLOR_RED); }


		if (TestAlignedAlloc1()) { LOGC_TEST("Testing UnitTestPoolAlloc::TestAlignedAlloc1: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestPoolAlloc::TestAlignedAlloc1: failed", COLOR_RED); }

		if (TestAlignedAlloc2()) { LOGC_TEST("Testing UnitTestPoolAlloc::TestAlignedAlloc2: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestPoolAlloc::TestAlignedAlloc2: failed", COLOR_RED); }

		if (TestFreeAligned1()) { LOGC_TEST("Testing UnitTestPoolAlloc::TestFreeAligned1: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestPoolAlloc::TestFreeAligned1: failed", COLOR_RED); }

		if (TestFreeAligned2()) { LOGC_TEST("Testing UnitTestPoolAlloc::TestFreeAligned2: successful", COLOR_GREEN); }
		else { LOGC_TEST("Testing UnitTestPoolAlloc::TestFreeAligned2: failed", COLOR_RED); }
	}

	void UnitTestPoolAlloc::Cleanup()
	{
		alloc1.Clear();
		alloc2.Clear();
		alloc3.Clear();
		alloc4.Clear();

		alloc1.~PoolAllocator();
		alloc2.~PoolAllocator();
		alloc3.~PoolAllocator();
		alloc4.~PoolAllocator();
	}

	bool UnitTestPoolAlloc::TestAlloc1() noexcept
	{
		bool check1 = alloc1.GetUsedMem() == 0;

		p_testAlloc1 = alloc1.Alloc<TestStruct1>();
		p_testAlloc1->a = 8;
		p_testAlloc1->b = 12;
		p_testAlloc1->c = p_testAlloc1->a + p_testAlloc1->b;

		bool check2 = p_testAlloc1->c == 20;

		p_testAlloc2 = alloc1.Alloc<TestStruct2>();
		p_testAlloc2->a = 0.25f;

		bool check3 = p_testAlloc2->a == 0.25f;
		bool check4 = alloc1.GetNumAllocs() == 2;
		bool check5 = alloc1.GetNumFreePoolElements() == 10;
		bool check6 = alloc1.GetUsedMem() == 40;

		return check1 && check2 && check3 && check4 && check5 && check6;
	}

	bool UnitTestPoolAlloc::TestAlloc2() noexcept
	{
		bool check1 = alloc2.GetUsedMem() == 0;
		bool check2 = alloc2.GetNumPoolElements() == alloc2.GetNumFreePoolElements();

		p_testAlloc3 = alloc2.Alloc<TestStruct3>();

		bool check3 = alloc2.GetNumAllocs() == 1;
		bool check4 = alloc2.GetNumFrees() == 0;
		bool check5 = alloc2.GetUsedMem() == 128;
		bool check6 = alloc2.GetNumPoolElements() == 8;
		bool check7 = alloc2.GetPoolSize() == 128;

		return check1 && check2 && check3 && check4 
			&& check5 && check6 && check7;
	}

	bool UnitTestPoolAlloc::TestAlloc3() noexcept
	{
		bool check1 = alloc3.GetUsedMem() == 0;

		p_testAlloc1_2 = alloc3.Alloc<TestStruct1>();

		bool check2 = alloc3.GetNumAllocs() == 1;
		bool check3 = alloc3.GetFreeMem() == alloc3.GetTotalMem() - 8;

		p_testAlloc2_2 = alloc3.Alloc<TestStruct2>();

		bool check4 = alloc3.GetNumAllocs() == 2;
		bool check5 = alloc3.GetFreeMem() == alloc3.GetTotalMem() - 40;

		return check1 && check2 && check3 && check4 && check5;
	}

	bool UnitTestPoolAlloc::TestAlloc4() noexcept
	{
		bool check1 = alloc4.GetFreeMem() == 64;

		p_testAlloc2_3 = alloc4.Alloc<TestStruct2>();
		bool check2 = alloc4.GetNumFreePoolElements() == 1;

		p_testAlloc2_4 = alloc4.Alloc<TestStruct2>();
		bool check3 = alloc4.GetUsedMem() == 64;

		alloc4.Free((intptr_t)p_testAlloc2_3, sizeof(TestStruct2));
		bool check4 = alloc4.GetNumFrees() == 1;

		p_testAlloc2_5 = alloc4.Alloc<TestStruct2>();
		bool check5 = alloc4.GetNumFreePoolElements() == 0;

		return check1 && check2 && check3 && check4 && check5;
	}

	bool UnitTestPoolAlloc::TestFree1() noexcept
	{
		bool check1 = alloc1.GetNumFreePoolElements() == 10;
		alloc1.Free((intptr_t)p_testAlloc1,sizeof(TestStruct1));

		bool check2 = alloc1.GetNumFreePoolElements() == 11;
		alloc1.Free((intptr_t)p_testAlloc2, sizeof(TestStruct2));

		bool check3 = alloc1.GetFreeMem() == alloc1.GetTotalMem();

		bool check4 = alloc1.GetNumFrees() == 2;
		bool check5 = alloc1.GetNumAllocs() == alloc1.GetNumFrees();

		return check1 && check2 && check3 && check4 && check5;
	}

	bool UnitTestPoolAlloc::TestFree2() noexcept
	{
		bool check1 = alloc2.GetFreeMem() != alloc2.GetTotalMem();
		bool check2 = alloc2.GetNumFreePoolElements() != alloc2.GetNumPoolElements();
		bool check3 = alloc2.GetUsedMem() == 128;

		alloc2.Free((uintptr_t)p_testAlloc3, sizeof(TestStruct3));
		
		bool check4 = alloc2.GetFreeMem() == 8 * 128;
		bool check5 = alloc2.GetNumFreePoolElements() == 8;

		return check1 && check2 && check3 && check4 && check5;
	}

	bool UnitTestPoolAlloc::TestFree3() noexcept
	{
		bool check1 = alloc3.GetUsedMem() == 40;
		bool check2 = alloc3.GetNumFreePoolElements() == 
			alloc3.GetNumPoolElements() - alloc3.GetNumAllocs();

		alloc3.Free((intptr_t)p_testAlloc1_2, sizeof(TestStruct1));
		bool check3 = alloc3.GetNumFrees() == 1;
		bool check4 = alloc3.GetNumFreePoolElements() == 
			alloc3.GetNumPoolElements() - 1;

		alloc3.Free((intptr_t)p_testAlloc2_2, sizeof(TestStruct2));
		bool check5 = alloc3.GetFreeMem() == alloc3.GetTotalMem();

		return check1 && check2 && check3 && check4 && check5;
	}

	bool UnitTestPoolAlloc::TestFree4() noexcept
	{
		bool check1 = alloc4.GetNumFrees() == 1;
		bool check2 = alloc4.GetNumPoolElements() == 2;
		bool check3 = alloc4.GetPoolSize() == 32;

		alloc4.Free((intptr_t)p_testAlloc2_4, sizeof(TestStruct2));

		bool check4 = alloc4.GetFreeMem() == alloc4.GetTotalMem() - 32;
		bool check5 = alloc4.GetNumFrees() == 2;
		bool check6 = alloc4.GetNumFreePoolElements() == 1;

		alloc4.Free((intptr_t)p_testAlloc2_5, sizeof(TestStruct2));

		bool check7 = alloc4.GetUsedMem() == 0;
		bool check8 = alloc4.GetNumFreePoolElements() == 2;

		return check1 && check2 && check3 && check4 && check5
			&& check6 && check7 && check8;
	}

	bool UnitTestPoolAlloc::TestClear1() noexcept
	{
		CCMemory::PoolAllocator ta = CCMemory::PoolAllocator(12, 64);

		for (int i = 0; i < 10; i++)
		{
			ta.Alloc <TestStruct2>();
		}

		bool check1 = ta.GetNumAllocs() == 10;
		bool check2 = ta.GetUsedMem() == 320;
		bool check3 = ta.GetNumFreePoolElements() == 2;

		ta.Clear();

		bool check4 = ta.GetFreeMem() == ta.GetTotalMem();
		bool check5 = ta.GetNumFreePoolElements() == 12;

		return check1 && check2 && check3 && check4 && check5;
	}

	bool UnitTestPoolAlloc::TestClear2() noexcept
	{
		CCMemory::PoolAllocator ta = CCMemory::PoolAllocator(80, 128);

		for (int i = 0; i < 50; i++)
		{
			ta.Alloc <TestStruct3>();
		}

		bool check1 = ta.GetNumAllocs() == 50;
		bool check2 = ta.GetUsedMem() == 128 * 50;
		bool check3 = ta.GetNumFreePoolElements() == 30;

		ta.Clear();

		bool check4 = ta.GetFreeMem() == ta.GetTotalMem();
		bool check5 = ta.GetNumFreePoolElements() == 80;
		//TODO: Define if this should be set to numAllocs (memory leak detection)
		bool check6 = ta.GetNumFrees() == 0;

		return check1 && check2 && check3 && check4 && check5 && check6;
	}

	bool UnitTestPoolAlloc::TestClear3() noexcept
	{
		CCMemory::PoolAllocator ta = CCMemory::PoolAllocator(8, 16);

		bool check1 = ta.GetFreeMem() == 8 * 16;
		bool check2 = ta.GetNumFreePoolElements() == 8;

		ta.Alloc<TestStruct1>();
		ta.Alloc<TestStruct1>();
		ta.Alloc<TestStruct1>();

		bool check3 = ta.GetNumFreePoolElements() == 5;

		ta.Clear();

		bool check4 = ta.GetFreeMem() == 8 * 16;
		bool check5 = ta.GetTotalMem() == ta.GetFreeMem();

		return check1 && check2 && check3 && check4 && check5;
	}

	bool UnitTestPoolAlloc::TestClear4() noexcept
	{
		CCMemory::PoolAllocator ta = CCMemory::PoolAllocator(4, 32);

		ta.Alloc<TestStruct2>();
		ta.Alloc<TestStruct2>();
		ta.Alloc<TestStruct2>();
		ta.Alloc<TestStruct2>();

		bool check1 = ta.GetFreeMem() == 0;
		bool check2 = ta.GetNumFreePoolElements() == 0;

		ta.Clear();

		bool check3 = ta.GetFreeMem() == 4 * 32;
		bool check4 = ta.GetNumFreePoolElements() == 4;
		bool check5 = ta.GetUsedMem() == 0;

		return check1 && check2 && check3 && check4 && check5;
	}

	bool UnitTestPoolAlloc::TestGetTotalMem() noexcept
	{
		CCMemory::PoolAllocator ta = CCMemory::PoolAllocator(4, 32);

		bool check1 = ta.GetTotalMem() == 4 * 32;

		ta.Alloc<TestStruct1>();

		bool check2 = ta.GetTotalMem() == 4 * 32;

		ta.Alloc<TestStruct1>();
		ta.Alloc<TestStruct1>();
		ta.Alloc<TestStruct1>();

		CCMemory::PoolAllocator ta2 = CCMemory::PoolAllocator(8, 256);

		ta2.Alloc<TestStruct3>();
		ta2.Alloc<TestStruct3>();

		bool check3 = ta.GetTotalMem() == 4 * 32;
		bool check4 = ta2.GetTotalMem() == 8 * 256;

		return check1 && check2 && check3 && check4;
	}

	bool UnitTestPoolAlloc::TestGetUsedMem() noexcept
	{
		CCMemory::PoolAllocator ta = CCMemory::PoolAllocator(4, 32);

		bool check1 = ta.GetUsedMem() == 0;

		ta.Alloc<TestStruct1>();

		bool check2 = ta.GetUsedMem() == 8;

		ta.Alloc<TestStruct1>();
		ta.Alloc<TestStruct1>();
		ta.Alloc<TestStruct1>();

		CCMemory::PoolAllocator ta2 = CCMemory::PoolAllocator(8, 256);

		ta2.Alloc<TestStruct3>();
		ta2.Alloc<TestStruct3>();

		bool check3 = ta.GetUsedMem() == 32;
		bool check4 = ta2.GetUsedMem() == 256;

		return check1 && check2 && check3 && check4;
	}

	bool UnitTestPoolAlloc::TestGetFreeMem() noexcept
	{
		CCMemory::PoolAllocator ta = CCMemory::PoolAllocator(4, 32);

		bool check1 = ta.GetFreeMem() == ta.GetTotalMem();

		ta.Alloc<TestStruct1>();

		bool check2 = ta.GetFreeMem() == 4 * 32 - 8;

		ta.Alloc<TestStruct1>();
		ta.Alloc<TestStruct1>();
		ta.Alloc<TestStruct1>();

		CCMemory::PoolAllocator ta2 = CCMemory::PoolAllocator(8, 256);

		ta2.Alloc<TestStruct3>();
		ta2.Alloc<TestStruct3>();

		bool check3 = ta.GetFreeMem() == (4 * 32) - (4 * 8);
		bool check4 = ta2.GetFreeMem() == 7 * 256;

		return check1 && check2 && check3 && check4;
	}

	bool UnitTestPoolAlloc::TestGetNumAllocs() noexcept
	{
		CCMemory::PoolAllocator ta = CCMemory::PoolAllocator(128, 64);
		
		for (int i = 0; i < 120; i++)
		{
			ta.Alloc<TestStruct2>();
		}

		bool check1 = ta.GetNumAllocs() == 120;

		ta.Clear();

		for (int i = 0; i < 29; i++)
		{
			ta.Alloc<TestStruct1>();
		}

		bool check2 = ta.GetNumAllocs() == 149;

		CCMemory::PoolAllocator ta2 = CCMemory::PoolAllocator(12, 8);

		ta2.Alloc<TestStruct1>();
		ta2.Alloc<TestStruct1>();

		bool check3 = ta2.GetNumAllocs() == 2;

		for (int i = 0; i < 41; i++)
		{
			ta.Alloc<TestStruct1>();
		}

		bool check4 = ta.GetNumAllocs() == 190;

		return check1 && check2 && check3 && check4;
	}

	bool UnitTestPoolAlloc::TestGetNumFrees() noexcept
	{
		CCMemory::PoolAllocator ta = CCMemory::PoolAllocator(128, 64);

		TestStruct2* ptrs[120];

		for (int i = 0; i < 120; i++)
		{
			ptrs[i] = ta.Alloc<TestStruct2>();
		}

		for (int i = 0; i < 120; i++)
		{
			ta.Free((intptr_t)ptrs[i], sizeof(TestStruct2));
		}

		bool check1 = ta.GetNumFrees() == 120;

		ta.Clear();

		TestStruct1* ptrs2[29];

		for (int i = 0; i < 29; i++)
		{
			ptrs2[i] = ta.Alloc<TestStruct1>();
		}

		for (int i = 0; i < 29; i++)
		{
			ta.Free((intptr_t) ptrs2[i], sizeof(TestStruct1));
		}

		bool check2 = ta.GetNumFrees() == 149;

		CCMemory::PoolAllocator ta2 = CCMemory::PoolAllocator(12, 8);

		auto* one = ta2.Alloc<TestStruct1>();
		auto* two = ta2.Alloc<TestStruct1>();

		ta2.Free((intptr_t)one, sizeof(TestStruct1));
		ta2.Free((intptr_t)two, sizeof(TestStruct1));

		bool check3 = ta2.GetNumFrees() == 2;

		TestStruct1* ptrs3[41];
		for (int i = 0; i < 41; i++)
		{
			ptrs3[i] = ta.Alloc<TestStruct1>();
		}

		for (int i = 0; i < 41; i++)
		{
			ta.Free((intptr_t)ptrs3[i],sizeof(TestStruct1));
		}

		bool check4 = ta.GetNumFrees() == 190;

		return check1 && check2 && check3 && check4;
	}

	bool UnitTestPoolAlloc::TestAlignedAlloc1() noexcept
	{
		bool check1 = alloc7.GetUsedMem() == 0;

		p_testAlloc1 = alloc7.AllocAligned<TestStruct1>();
		p_testAlloc1->a = 8;
		p_testAlloc1->b = 12;
		p_testAlloc1->c = p_testAlloc1->a + p_testAlloc1->b;

		bool check2 = p_testAlloc1->c == 20;
		bool check3 = (intptr_t)p_testAlloc1 % sizeof(TestStruct1) == 0;

		p_testAlloc2 = alloc7.AllocAligned<TestStruct2>();
		p_testAlloc2->a = 0.25f;

		bool check4 = p_testAlloc2->a == 0.25f;
		bool check5 = alloc7.GetNumAllocs() == 2;
		bool check6 = alloc7.GetNumFreePoolElements() == 6;
		bool check7 = alloc7.GetUsedMem() == 40;
		bool check8 = (intptr_t)p_testAlloc2 % sizeof(TestStruct2) == 0;

		return check1 && check2 && check3 && check4 && check5 && check6 && check7
			&& check8;
	}

	bool UnitTestPoolAlloc::TestAlignedAlloc2() noexcept
	{
		alloc7.Clear();
		bool check1 = alloc7.GetUsedMem() == 0;
		bool check2 = alloc7.GetNumPoolElements() == alloc7.GetNumFreePoolElements();

		p_testAlloc3 = alloc7.AllocAligned<TestStruct3>();

		bool check3 = alloc7.GetNumAllocs() == 3;
		bool check4 = alloc7.GetNumFrees() == 0;
		bool check5 = alloc7.GetUsedMem() == 128;
		bool check6 = alloc7.GetNumPoolElements() == 8;
		bool check7 = alloc7.GetPoolSize() == 256;
		bool check8 = (intptr_t)p_testAlloc3 % sizeof(TestStruct3) == 0;

		return check1 && check2 && check3 && check4
			&& check5 && check6 && check7 && check8;
	}

	bool UnitTestPoolAlloc::TestFreeAligned1() noexcept
	{
		TestStruct2* p_testAlloc5 = alloc5.AllocAligned<TestStruct2>();
		TestStruct1* p_testAlloc4 = alloc5.AllocAligned<TestStruct1>();

		bool check1 = alloc5.GetNumFreePoolElements() == 8;
		alloc5.Free((intptr_t)p_testAlloc5, sizeof(TestStruct2));


		bool check2 = alloc5.GetNumFreePoolElements() == 9;
		alloc5.Free((intptr_t)p_testAlloc4, sizeof(TestStruct1));

		bool check3 = alloc5.GetFreeMem() == alloc5.GetTotalMem();

		bool check4 = alloc5.GetNumFrees() == 2;
		bool check5 = alloc5.GetNumAllocs() == alloc5.GetNumFrees();

		return check1 && check2 && check3 && check4 && check5;
	}

	bool UnitTestPoolAlloc::TestFreeAligned2() noexcept
	{
		TestStruct1* p_testAlloc3 = alloc6.AllocAligned<TestStruct1>();

		bool check1 = alloc6.GetFreeMem() != alloc6.GetTotalMem();
		bool check2 = alloc6.GetNumFreePoolElements() != alloc6.GetNumPoolElements();
		bool check3 = alloc6.GetUsedMem() == 8;

		alloc6.FreeAligned((uintptr_t)p_testAlloc3, sizeof(TestStruct1));

		bool check4 = alloc6.GetFreeMem() == 4 * 32;
		bool check5 = alloc6.GetNumFreePoolElements() == 4;

		return check1 && check2 && check3 && check4 && check5;
	}
}
