#include "String.h"
#include "../Utilities/CRCHash.h"

namespace CCE
{
	unsigned long long String::GetStringID(const char* str)
	{		
		return CCE::Math::CRCHash::HashValue(str);
	}
}
