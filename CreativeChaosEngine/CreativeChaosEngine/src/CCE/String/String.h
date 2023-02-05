#pragma once
#include "../Core.h"
#include <unordered_map>
#include <iterator>
#include "../Analysis/Debug.h"

namespace CCE
{
	// TODO: Implement something so that the struct constructor 
	// will return the string to the used function

	// TODO: Implement Logging for custom strings
	struct CCE_API String
	{
		String(const char* str = "")
		{
			// TODO: Save char* in specific memory and maybe only use
			// hashed values in the running game (GAE: 459)
			sId = GetStringID(str);

#ifdef DEBUG
			bool collision = gStringTable.find(sId) != gStringTable.end() 
				&& strcmp(gStringTable[sId], str) != 0;
			if (collision)
			{
				printf("Hash-Collision on strings '%s' and '%s'.", str, gStringTable[sId]);
			}
			DASSERT(!collision, "There has been a hash function collision!");
#endif // DEBUG

			// Add String if it doesn't exist already (copy str)
			gStringTable.try_emplace(sId, _strdup(str));
		}

		~String()
		{
			std::unordered_map<unsigned long long, const char*>::iterator it;

			// Free all the memory allocated by _strdup
			for (it = gStringTable.begin(); it != gStringTable.end(); it++)
			{
				free((void*)it->second);
			}
			gStringTable.clear();
		}

		String& operator=(const String& other)
		{
			// check if both refs are the same instance
			if (this == &other)
			{
				return *this;
			}

			this->sId = other.sId;
			return *this;
		}

		String& operator=(const char* other)
		{
			*this = String(other);
			return *this;
		}

		bool operator==(const String& other)
		{
			// compare strings by their string id
			return this->sId == other.sId;
		}

		bool operator!=(const String& other)
		{
			// compare strings by their string id
			return this->sId != other.sId;
		}

		const size_t Length();
		const char* Value();

	public:
		unsigned long long sId = 0;
		
	private:
		inline static std::unordered_map<unsigned long long, const char*> gStringTable;

	private:
		const unsigned long long GetStringID(const char* str);
	};
}
