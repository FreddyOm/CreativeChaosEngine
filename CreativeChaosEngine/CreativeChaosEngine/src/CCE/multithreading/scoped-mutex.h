#pragma once
#include <mutex>
#include "../manager/profilingManager.h"

class ScopedMutex
{
public:
	ScopedMutex(std::mutex& mutex)
		: m_mutex(&mutex)
	{
		OPTICK_EVENT();
		while (!mutex.try_lock())
			continue;
	}

	~ScopedMutex()
	{
		OPTICK_EVENT();
		m_mutex->unlock();
		m_mutex = nullptr;
	}

private:
	std::mutex* m_mutex{};
};

