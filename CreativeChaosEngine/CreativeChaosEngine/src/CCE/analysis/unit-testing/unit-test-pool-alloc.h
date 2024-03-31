#pragma once
#include "unit-test.h"
#include "../../memory/pool-allocator.h"

namespace CCE_Testing
{
	struct CCE_API UnitTestPoolAlloc : public UnitTest
	{
		friend struct PoolAllocator;

		void Test() override;
		void Cleanup() override;
	
	private:

		bool TestAlloc1() noexcept;
		bool TestAlloc2() noexcept;
		bool TestAlloc3() noexcept;
		bool TestAlloc4() noexcept;

		bool TestAlignedAlloc1() noexcept;
		bool TestAlignedAlloc2() noexcept;

		bool TestFree1() noexcept;
		bool TestFree2() noexcept;
		bool TestFree3() noexcept;
		bool TestFree4() noexcept;

		bool TestClear1() noexcept;
		bool TestClear2() noexcept;
		bool TestClear3() noexcept;
		bool TestClear4() noexcept;

		bool TestGetTotalMem() noexcept;
		bool TestGetUsedMem() noexcept;
		bool TestGetFreeMem() noexcept;
		bool TestGetNumAllocs() noexcept;
		bool TestGetNumFrees() noexcept;

		bool TestFreeAligned1() noexcept;
		bool TestFreeAligned2() noexcept;

	private:

		CCMemory::PoolAllocator alloc1 = CCMemory::PoolAllocator(12, 64);
		CCMemory::PoolAllocator alloc2 = CCMemory::PoolAllocator(8, 128);
		CCMemory::PoolAllocator alloc3 = CCMemory::PoolAllocator(32, 256);
		CCMemory::PoolAllocator alloc4 = CCMemory::PoolAllocator(2, 32);
		CCMemory::PoolAllocator alloc5 = CCMemory::PoolAllocator(10, 128);
		CCMemory::PoolAllocator alloc6 = CCMemory::PoolAllocator(4, 32);
		CCMemory::PoolAllocator alloc7 = CCMemory::PoolAllocator(8, 256);
		
		struct TestStruct1 // size = 8 bytes
		{
			char a = 0;
			char b = 0;
			char c = 0;
			char d = 0;

			char e = 0;
			char f = 0;
			char g = 0;
			char h = 0;
		};

		struct TestStruct2 // size = 32 bytes
		{
			float a = 0.0f;
			float b = 0.0f;
			float c = 0.0f;
			float d = 0.0f;

			int e = 0;
			int f = 0;
			int g = 0;
			int h = 0;
		};

		struct TestStruct3 // size = 128 bytes
		{
			TestStruct2 ts1 = { 0 };
			TestStruct2 ts2 = { 0 };
			TestStruct2 ts3 = { 0 };
			TestStruct2 ts4 = { 0 };
		};

		TestStruct1* p_testAlloc1 = nullptr;
		TestStruct2* p_testAlloc2 = nullptr;
		TestStruct3* p_testAlloc3 = nullptr;
		TestStruct1* p_testAlloc1_2 = nullptr;
		TestStruct2* p_testAlloc2_2 = nullptr;
		TestStruct2* p_testAlloc2_3 = nullptr;
		TestStruct2* p_testAlloc2_4 = nullptr;
		TestStruct2* p_testAlloc2_5 = nullptr;
	};
}
