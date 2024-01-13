#pragma once
#include "../Core.h"
#include "../String/String.h"
#include <vector>
#include <memory>

namespace CCE::Resources
{
	template<typename T>
	struct CCE_API ResourceLoader
	{
		virtual std::shared_ptr<T> LoadResource(String filePath) = 0;
	};
}
