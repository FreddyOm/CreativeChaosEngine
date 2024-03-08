#pragma once
#include "ScopedSpinLock.h"
#include "SpinLock.h"
#include <functional>
#include <winternl.h>
#include <thread>


#define JOB_RETURN_TYPE void

typedef std::function<JOB_RETURN_TYPE(va_list)> EntryPoint;
typedef std::atomic<int> Counter;

struct Job
{
	EntryPoint m_pEntryPoint;
	LPVOID m_pFiber = NULL;
	Counter* m_pCounter;
	unsigned int mDesiredCount = 0;

};

