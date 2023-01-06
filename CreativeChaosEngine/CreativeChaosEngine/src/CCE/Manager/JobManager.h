#pragma once
#include "BaseManager.h"

namespace CCE
{
	struct CCE_API JobManager : public BaseManager
	{
	public:
		JobManager() = default;
		~JobManager() = default;

		void StartUp() override;
		void ShutDown() override;
	};
}
