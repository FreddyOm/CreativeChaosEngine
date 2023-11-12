#include "StringMemory.h"
#include "../Analysis/Debug.h"
#include "../Utilities/Math/CRCHash.h"

namespace CCE
{
	/// <summary>
	/// Query the string value (char*) with the given handle.
	/// </summary>
	/// <param name="stringHandle"></param>
	/// <returns>The raw pointer to the requested string.</returns>
	char* StringMemory::GetValue(UINT64 stringHandle)
	{
		return FindStrMemDesc(stringHandle)->pStringBuf;
	}

	UINT64 StringMemory::GetLength(UINT64 stringHandle)
	{
		return FindStrMemDesc(stringHandle)->length;
	}

	/// <summary>
	/// Creates a string in string memory or increases ref-count.
	/// </summary>
	/// <param name="pString">The string pointer.</param>
	/// <returns>Returns handle of the string if successfully added, -1 if failed./returns>
	UINT64 StringMemory::CreateString(const char* pString)
	{
		DASSERT(pString != nullptr, "String may not be a nullptr!");

		UINT64 sHndl = GetStringID(pString);
		UINT64 strLen = strlen(pString);

		StringMemDescritpion* smd = FindStrMemDesc(sHndl);

		if (smd != nullptr)
		{
			// If string already exists, increase refcount
			smd->refCount++;
		}
		else 
		{
			// If string is not in table, add it to table
			strHndlDesc.push_back(
				StringMemDescritpion(sHndl, 1, bufPtr, strLen)
			);
			
			memcpy(bufPtr, std::move(pString), strLen); // Append string to buffer 
			bufPtr += strLen + 1; // Push buffer ptr by the size of the added string

			DASSERT(bufPtr < &stringBuf[STRING_BUF_LEN],
				"String Memory Buffer Overflow!");
		}
		
		return bufPtr < &stringBuf[STRING_BUF_LEN] ? sHndl : -1;
	}

	/// <summary>
	/// Removes a string if there is no other reference on it.
	/// </summary>
	/// <param name="stringHandle">The handle to the string.</param>
	void StringMemory::DestroyString(UINT64 stringHandle)
	{
		StringMemDescritpion* smd = FindStrMemDesc(stringHandle);
		//DASSERT(smd != nullptr,"Invalid stringHandle");    // Might destroy invalid strings due to move construction!
		if (smd == nullptr) { return; }

		// Remove string if no ref exists anymore
		if (--smd->refCount <= 0)
		{
			// Override string by defragmenting string buffer 
			bufPtr = DefragmentBuffer(smd->pStringBuf, smd->length);
			
			// Remove string mem descr ref
			std::vector<StringMemDescritpion>::iterator it = strHndlDesc.begin();
			bool _del = false; // Cache deletion in case while runs to end without breaking

			while (it != strHndlDesc.end())
			{
				if (it->handle == smd->handle)
				{
					_del = true;
					break;
				}
				it++;
			}

			if (_del) { strHndlDesc.erase(it); }
		}
	}

	/// <summary>
	/// Returns a hash value that represents the string value.
	/// </summary>
	/// <param name="str">A pointer to the string.</param>
	/// <returns>The hashed string.</returns>
	UINT64 StringMemory::GetStringID(const char* str)
	{
		return CCE::Math::CRCHash::HashValue(str, strlen(str));
	}

	/// <summary>
	/// Finds the string memory description object with the given handle.
	/// </summary>
	/// <param name="stringHandle">Returns a pointer to the object or nullptr if not found.</param>
	/// <returns></returns>
	StringMemory::StringMemDescritpion* StringMemory::FindStrMemDesc(UINT64 stringHandle)
	{
		for (auto& smd : strHndlDesc)
		{
			if (smd.handle == stringHandle) { return &smd; }
		}

		return nullptr;
	}

	/// <summary>
	/// Shifts all memory starting at pos ending at the end of the string mem buf
	/// to the left by the length of the gap.
	/// </summary>
	/// <param name="pos">The position of the removed object.</param>
	/// <param name="shift">The length of the gap.</param>
	/// 
	
	/// <summary>
	/// Shifts all memory starting at pos ending at the end of the string mem buf
	/// to the left by the length of the gap.
	/// </summary>
	/// <param name="pos">The position of the removed object.</param>
	/// <param name="shift">The length of the gap.</param>
	/// <returns>The new top buffer pointer.</returns>
	char* StringMemory::DefragmentBuffer(const char* pos, size_t shift)
	{
		// TODO: Maybe do this every other frame generically instead of 
		// when a string is deconstructed
	
		++shift; // This is necessary in order to include the null terminator in the removal.

		for (size_t i = 0; i < strHndlDesc.size(); i++)
		{
			if (strHndlDesc.at(i).pStringBuf > pos)
			{
				strHndlDesc.at(i).pStringBuf -= shift;
			}
		}
		// Calc num of bytes from begin of strBuf to pos
		unsigned int strBufIndex = (uintptr_t)pos - (uintptr_t)&stringBuf[0];
		memmove((void*)pos, pos + shift, STRING_BUF_LEN - (strBufIndex + shift));

		// TODO: Evaluate if this is necessary
		ZeroMemory(&stringBuf[STRING_BUF_LEN - shift], shift);
		return (bufPtr - shift);
	}

	char StringMemory::stringBuf[STRING_BUF_LEN] = { 0 };
	alignas(32) std::vector<StringMemory::StringMemDescritpion> StringMemory::strHndlDesc = {};
	char* StringMemory::bufPtr = &stringBuf[0];
}
