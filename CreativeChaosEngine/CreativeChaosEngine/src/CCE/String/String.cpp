#include "String.h"

namespace CCE
{
	size_t String::GetStringID(const char* str)
	{
		//TODO: insert better hash function
		using namespace std;
		hash<string> _hash;
		
		return _hash(string(str));
	}
}
