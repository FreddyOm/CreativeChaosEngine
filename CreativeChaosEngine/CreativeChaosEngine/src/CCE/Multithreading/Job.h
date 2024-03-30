#pragma once
#include "scoped-spinlock.h"
#include "spinlock.h"
#include <functional>

namespace CCE::Jobs
{
	typedef void JobReturnType;
	typedef JobReturnType(*JobEntryPoint)(uintptr_t args);
	typedef std::atomic<int> Counter;

	enum class alignas(4) Priority : byte
	{
		LOW = 0,
		NORMAL = 1,
		HIGH = 2,
		CRITICAL = 3,
	};

	/// <summary>
	/// A structure used to describe a job which should be executed 
	/// at a given point in time.
	/// </summary>
	struct Job
	{
		JobEntryPoint m_EntryPoint = nullptr;	// 64 bytes
		LPVOID m_Fiber = NULL;					// 8 bytes
		uintptr_t m_Param = NULL;				// 8 bytes
		Counter* m_pCounter;					// 8 bytes

		unsigned int m_DesiredCount = 0;		// 4 bytes
		Priority m_Priority = (Priority)1;		// 4 bytes

		byte padding[32] = { 0 };				// 32 bytes

		Job()
			: m_EntryPoint(nullptr)
		{ }
		Job(void* ep, Priority pr, uintptr_t args = 0)
			: m_EntryPoint(static_cast<JobEntryPoint>(ep)), m_Param(args), m_Priority(pr)
		{ }

		// Copy semantics
		Job(const Job& other) = delete;
		Job& operator=(const Job& other) = delete;

		// Move semantics
		Job(Job&& other) noexcept
			: m_EntryPoint(other.m_EntryPoint)
			, m_Fiber(other.m_Fiber)
			, m_Param(other.m_Param)
			, m_pCounter(other.m_pCounter)
			, m_DesiredCount(other.m_DesiredCount)
			, m_Priority(other.m_Priority)
		{
			other.m_EntryPoint = nullptr;
			other.m_Fiber = NULL;
			other.m_Param = NULL;
			other.m_pCounter = nullptr;
			other.m_DesiredCount = 0;
			other.m_Priority = Priority::NORMAL;
		}
		Job& operator=(Job&& other) noexcept
		{
			m_EntryPoint = other.m_EntryPoint;
			m_Fiber = other.m_Fiber;
			m_Param = other.m_Param;
			m_pCounter = other.m_pCounter;
			m_DesiredCount = other.m_DesiredCount;
			m_Priority = other.m_Priority;

			other.m_EntryPoint = nullptr;
			other.m_Fiber = NULL;
			other.m_Param = NULL;
			other.m_pCounter = nullptr;
			other.m_DesiredCount = 0;
			other.m_Priority = Priority::NORMAL;

			return *this;
		}
	};

}
