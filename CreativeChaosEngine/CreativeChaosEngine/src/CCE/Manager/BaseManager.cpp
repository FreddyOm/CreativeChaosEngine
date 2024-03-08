#include "BaseManager.h"

namespace CCE
{
	void BaseManager::Init() const
	{
		initialized.store(true);
	}

	void BaseManager::Deinit() const
	{
		initialized.store(false);
	}

	bool BaseManager::IsInitialized() const
	{
		return initialized.load(std::memory_order_consume);
	}
}