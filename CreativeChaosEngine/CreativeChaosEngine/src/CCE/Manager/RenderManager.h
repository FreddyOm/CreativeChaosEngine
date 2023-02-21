#pragma once
#include "BaseManager.h"

namespace CCE
{
	struct CCE_API RenderManager : public BaseManager
	{
	public:
		RenderManager() = default;
		~RenderManager() = default;

		void StartUp() override;
		void ShutDown() override;

		static RenderManager* Instance;
	};
}
