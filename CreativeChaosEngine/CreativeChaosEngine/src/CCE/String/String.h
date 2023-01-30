#pragma once
#include "../Core.h"
#include <unordered_map>
#include <string>

namespace CCE
{
	//TODO: Implement something so that the struct constructor 
	// will return the string to the used function
	
	//TODO: Add Unit tests!
	struct CCE_API String
	{
		String(const char* str = "")
		{
			// Add String if it doesn't exist already
			strings.try_emplace(GetStringID(str), std::string(str));
		}

		~String()
		{
			strings.clear();
		}

	private:
		inline static std::unordered_map<size_t, std::string> strings;

	private:
		size_t GetStringID(const char* str);
	};
}
