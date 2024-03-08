#pragma once
#include "ScopedSpinLock.h"
#include "SpinLock.h"
#include <functional>

#define JOB_RETURN_TYPE void

typedef std::function<JOB_RETURN_TYPE(va_list)> EntryPoint;
typedef std::atomic<int> Counter;

enum class alignas(4) Priority : byte
{
	LOW = 0,
	NORMAL = 1,
	HIGH = 2
};

/// <summary>
/// A structure used to describe a job which should be executed 
/// at a given point in time.
/// </summary>
struct alignas(128) Job
{
	EntryPoint m_EntryPoint;			// 64 bytes
	LPVOID m_Fiber = NULL;				// 8 bytes
	va_list m_Param = NULL;				// 8 bytes
	Counter* m_pCounter;				// 8 bytes

	unsigned int m_DesiredCount = 0;	// 4 bytes
	Priority m_Priority = (Priority)1;	// 4 bytes

	byte padding[32] = {0};				// 32 bytes

	Job()
		: m_EntryPoint(nullptr)
	{ }
	Job(const EntryPoint& ep, Priority pr, ...)
		: m_EntryPoint(ep), m_Priority(pr)
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
