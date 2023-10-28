#include "String.h"
#include "../Utilities/Math/CRCHash.h"
#include "../Manager/MemoryManager.h"
#include "../Manager/ProfilingManager.h"

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
	{ }

	/// <summary>
	/// Clears the global string table.
	/// </summary>
	void String::ClearGlobalStringTable()
	{
		std::unordered_map<UINT64, const char*>::iterator it;

		// Free all the memory allocated by _strdup
		for (it = gStringTable.begin(); it != gStringTable.end(); it++)
		{
			free((void*)it->second);
		}
		gStringTable.clear();
	}

	/// <summary>
	/// Returns the length of the string.
	/// </summary>
	/// <returns></returns>
	UINT64 String::Length() const
	{
		if (gStringTable[sId] == NULL)
		{
			DERROR(GetLastError());
			return 0;
		}

		return strlen(gStringTable[sId]);
	}

	/// <summary>
	/// Checks wheather the string is empty.
	/// </summary>
	/// <returns></returns>
	bool String::IsEmpty(String& str)
	{
		return str.Length() == 0;
	}

	/// <summary>
	/// Returns the actual string value of a given string id.
	/// </summary>
	/// <returns></returns>
	const char* String::Value() const
	{
		return gStringTable[sId] == NULL ? "" : gStringTable[sId];
	}

	/// <summary>
	/// Returns the string that matches a given sId;
	/// </summary>
	/// <param name="sId">The ID to the string to be found.</param>
	/// <returns>The string that matches the ID. NULL if no string is found.</returns>
	const char* String::ValueBySID(UINT64 sId)
	{
		return gStringTable[sId] == NULL ? "" : gStringTable[sId];
	}

	const UINT64 String::GetStringID(const char* str)
	{		
		return CCE::Math::CRCHash::HashValue(str, strlen(str));
	}

	std::unordered_map<UINT64, const char*> String::gStringTable;
}
