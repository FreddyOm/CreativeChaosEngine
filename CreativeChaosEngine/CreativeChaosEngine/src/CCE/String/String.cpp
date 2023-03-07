#include "String.h"
#include "../Utilities/CRCHash.h"

namespace CCE
{
	String::String(const char* str)
	{
		// TODO: Save char* in specific memory and maybe only use
		// hashed values in the running game (GAE: 459)
		sId = GetStringID(str);

#ifdef DEBUG
		bool collision = gStringTable.find(sId) != gStringTable.end()
			&& strcmp(gStringTable[sId], str) != 0;
		if (collision)
		{
			printf("Hash-Collision on strings \"%s\" and \"%s\".", str, gStringTable[sId]);
		}
		DASSERT(!collision, "There has been a hash function collision!");
#endif // DEBUG

		// Add String if it doesn't exist already (copy str)
		gStringTable.try_emplace(sId, _strdup(str));
	}

	String::~String()
	{
		std::unordered_map<unsigned long long, const char*>::iterator it;

		// Free all the memory allocated by _strdup
		for (it = gStringTable.begin(); it != gStringTable.end(); it++)
		{
			free((void*)it->second);
		}
		gStringTable.clear();
	}

	size_t String::Length() const
	{
		return strlen(gStringTable[sId]);
	}

	const char* String::Value() const
	{
		return gStringTable[sId] == NULL ? "" : gStringTable[sId];
	}

	const unsigned long long String::GetStringID(const char* str)
	{		
		return CCE::Math::CRCHash::HashValue(str, strlen(str));
	}

	std::unordered_map<unsigned long long, const char*> String::gStringTable;
}
