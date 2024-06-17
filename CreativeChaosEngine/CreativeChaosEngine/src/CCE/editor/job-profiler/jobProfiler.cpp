#include "jobProfiler.h"
#include <mutex>
#include "JobProfilingData.h"
#include "JobProfilingWaitData.h"

namespace CCE::Debug
{
	std::mutex jobProfileDataLock{};
	std::mutex currentJobProfileDataLock{};
	std::mutex waitingJobProfileDataLock{};
	
	std::unordered_map<DWORD, std::vector<JobProfilingData>> threadLocalFrameProfileData{};
	
	std::unordered_map<DWORD, JobProfilingData*> currentProfileData{};
	std::unordered_map<LPVOID, JobProfilingData*> waitingProfileData{};

	bool sampleJobs = false;
	Time::time::time_point frameStart{};

	void InitializeJobProfiler()
	{
		threadLocalFrameProfileData.reserve(30);
		currentProfileData.reserve(30);
		waitingProfileData.reserve(30);
	}

	void RegisterJobData(JobProfilingData&& data)
	{
		if (!sampleJobs) { return; }

		std::lock_guard guard(jobProfileDataLock);

		// Put it into the list
		if(threadLocalFrameProfileData.contains(GetCurrentThreadId()))
		{
			threadLocalFrameProfileData[GetCurrentThreadId()].push_back(std::move(data));
		}
		else
		{
			threadLocalFrameProfileData.emplace(GetCurrentThreadId(), std::vector<JobProfilingData>{data});
		}

		SetCurrentProfilingData(&threadLocalFrameProfileData[GetCurrentThreadId()].back());
	}

	void SetCurrentProfilingData(JobProfilingData* data)
	{
		if (!sampleJobs) { return; }

		std::lock_guard guard(currentJobProfileDataLock);

		if(currentProfileData.contains(GetCurrentThreadId()))
		{
			currentProfileData[GetCurrentThreadId()] = data;
		}
		else
		{
			currentProfileData.emplace(GetCurrentThreadId(), data);
		}
	}

	void ResetFrameSamples()
	{
		if (!sampleJobs) { return; }

		std::lock_guard guard1(jobProfileDataLock);
		std::lock_guard guard2(currentJobProfileDataLock);
		std::lock_guard guard3(waitingJobProfileDataLock);

		threadLocalFrameProfileData.clear();
		currentProfileData.clear();
		waitingProfileData.clear();
	}

	void SetJobExitTime(Time::time::time_point exitTime)
	{
		std::lock_guard guard2(currentJobProfileDataLock);

		if(currentProfileData.contains(GetCurrentThreadId()))
			currentProfileData[GetCurrentThreadId()]->jobExitTime = exitTime;
	}

	std::unordered_map<DWORD, std::vector<JobProfilingData>>& GetFrameProfilingData2()
	{
		std::lock_guard guard(jobProfileDataLock);
		return threadLocalFrameProfileData;
	}

	void PreSwitchFiber()
	{
		if (!sampleJobs) { return; }

		{
			std::lock_guard guard2(currentJobProfileDataLock);
			if(currentProfileData.contains(GetCurrentThreadId()) && currentProfileData[GetCurrentThreadId()] != NULL)
				currentProfileData[GetCurrentThreadId()]->profilingWaitData.push_back(JobProfilingWaitData());
		}

		{
			std::lock_guard guard(waitingJobProfileDataLock);
			waitingProfileData.emplace(GetCurrentFiber(), currentProfileData[GetCurrentThreadId()]);
		}
	}

	void PostSwitchFiber()
	{
		if (!sampleJobs) { return; }

		std::lock_guard guard(waitingJobProfileDataLock);
		
		LPVOID currentFiber = GetCurrentFiber();

		if (waitingProfileData.contains(currentFiber))
		{
			if (waitingProfileData[currentFiber] != NULL)
			{
				if(waitingProfileData[currentFiber]->profilingWaitData.size() > 0)
				{
					// Enter wait exit time
					waitingProfileData[currentFiber]->profilingWaitData.back().jobWaitExitTime = Time::Now();
				
					// Add new thread to list of all threads this job ran on
					waitingProfileData[currentFiber]->jobThreadId.push_back(GetCurrentThreadId());
				}

			}			
		}
		
		std::lock_guard guard2(currentJobProfileDataLock);

		// Override the current profile data on this thread
		if (currentProfileData.contains(GetCurrentThreadId()))
			currentProfileData[GetCurrentThreadId()] = waitingProfileData[GetCurrentFiber()];
		else
			currentProfileData.emplace(GetCurrentThreadId(), waitingProfileData[GetCurrentFiber()]);

		waitingProfileData.erase(GetCurrentFiber());
	}
}
