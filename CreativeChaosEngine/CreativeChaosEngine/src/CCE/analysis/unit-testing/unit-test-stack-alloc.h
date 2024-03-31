#pragma once
#include "unit-test.h"
#include "../../memory/stack-allocator.h"

namespace CCE_Testing
{
	struct CCE_API UnitTestStackAlloc : public UnitTest
	{
		friend struct StackAllocator;

		void Test() override;
		void Cleanup() override;

	private:
		bool TestAlloc1() noexcept;
		bool TestAlloc2() noexcept;
		bool TestAlloc3() noexcept;
		bool TestAlloc4() noexcept;
		bool TestFree1() noexcept;
		bool TestFree2() noexcept;
		bool TestFree3() noexcept;
		bool TestFree4() noexcept;
		bool TestClear1() noexcept;
		bool TestClear2() noexcept;
		bool TestClear3() noexcept;
		bool TestClear4() noexcept;
		bool TestMarker1() noexcept;
		bool TestMarker2() noexcept;
		bool TestMarker3() noexcept;
		bool TestMarker4() noexcept;
		bool TestGetTotalMem() noexcept;
		bool TestGetUsedMem() noexcept;
		bool TestGetFreeMem() noexcept;
		bool TestGetNumAllocs() noexcept;
		bool TestGetNumFrees() noexcept;
		bool TestGetCurrentTop() noexcept;
		bool TestAlignedAlloc1() noexcept;
		bool TestAlignedAlloc2() noexcept;
		bool TestAlignedFree1() noexcept;
		bool TestAlignedFree2() noexcept;

	};

	CCMemory::StackAllocator alloc1(1024); // 1024
	CCMemory::StackAllocator alloc2(32); // 32
	CCMemory::StackAllocator alloc3(2048000); // 2048000
	CCMemory::StackAllocator alloc4(8192); // 8192
	CCMemory::StackAllocator alloc5(256); // 265
	CCMemory::StackAllocator alloc6(8); // 8

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

}

