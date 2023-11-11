#include "String.h"
#include "../Manager/MemoryManager.h"
#include "../Manager/ProfilingManager.h"

namespace CCE
{
	String::String(const char* str)
	{
		sId = StringMemory::CreateString(str);
	}

	String::~String()
	{
		StringMemory::DestroyString(sId);
	}

	/// <summary>
	/// Returns the length of the string.
	/// </summary>
	/// <returns></returns>
	UINT64 String::Length() const
	{
		return StringMemory::GetLength(sId);
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
		return StringMemory::GetValue(sId);
	}

	/// <summary>
	/// Returns the string that matches a given sId;
	/// </summary>
	/// <param name="sId">The ID to the string to be found.</param>
	/// <returns>The string that matches the ID. NULL if no string is found.</returns>
	const char* String::ValueBySID(UINT64 sId)
	{
		return StringMemory::GetValue(sId);
	}
}
