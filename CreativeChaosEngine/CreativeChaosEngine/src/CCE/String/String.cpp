#include "String.h"
#include "../Utilities/CRCHash.h"

namespace CCE
{
	const size_t String::Length()
	{
		return strlen(gStringTable[sId]);
	}

	const char* String::Value()
	{
		return gStringTable[sId] == NULL ? "" : gStringTable[sId];
	}

	const unsigned long long String::GetStringID(const char* str)
	{		
		return CCE::Math::CRCHash::HashValue(str, strlen(str));
	}
}
