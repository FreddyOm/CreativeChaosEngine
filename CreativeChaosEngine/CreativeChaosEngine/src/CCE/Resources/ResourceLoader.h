#pragma once
#include "../Core.h"
#include "../String/String.h"
#include <vector>
#include <memory>
#include <unordered_map>

namespace CCE::Resources
{
	template<typename T>
	struct CCE_API ResourceLoader
	{
		virtual std::shared_ptr<T> LoadResource(String filePath) = 0;

		std::unordered_map<UINT64, std::shared_ptr<T>> DataResourceMap = {};
	};
}
