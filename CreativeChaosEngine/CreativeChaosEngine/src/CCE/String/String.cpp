#include "String.h"
#include "../Utilities/CRCHash.h"

namespace CCE
{
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
