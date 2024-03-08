#pragma once
#include "../Core.h"
#include <atomic>

namespace CCE
{
	struct BaseManager
	{
	public:
		BaseManager() noexcept {}
		~BaseManager() noexcept {}

		virtual void StartUp() = 0;
		virtual void ShutDown() = 0;

	protected:
		void Init() const;
		void Deinit() const;
		
		bool IsInitialized() const;

	private:
		mutable std::atomic<bool> initialized = false;
	};
}
