#include "String.h"
#include "../Utilities/Math/CRCHash.h"
#include "../Manager/MemoryManager.h"

namespace CCE
{
	String::String(const char* str)
	{
		// TODO: Save char* in specific memory and maybe only use
		// hashed values in the running game (GAE: 459)
		sId = GetStringID(str);

#ifdef DEBUG

		auto keyVal = gStringTable.find(sId);
		bool idInStringTable = keyVal != gStringTable.end(); // incoming string id is in string table
		
		if (idInStringTable) 
		{
			bool sEqual = keyVal->second == NULL ? false : (strcmp(keyVal->second, str) == 0); // strings are equal

			bool diff = sEqual && !idInStringTable;
			bool collision = !sEqual && idInStringTable;

			if (diff)
			{
				printf("Different hash values on strings \"%s\" and \"%s\".", str, gStringTable[sId]);
			}

			if (collision)
			{
				printf("Hash-Collision on strings \"%s\" and \"%s\".", str, gStringTable[sId]);
			}
			DASSERT(!collision, "There has been a hash function collision!");
			DASSERT(!diff, "There has been different hashes for the same string!");
		}
		
#endif // DEBUG

		// Add String if it doesn't exist already (move str)
		gStringTable.try_emplace(sId, std::move(str));
	}

	String::~String()
	{

	}

	void String::ClearGlobalStringTable()
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
		if (gStringTable[sId] == NULL)
		{
			DERROR(GetLastError());
			return -1;
		}
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
