#pragma once
#include "../Core.h"
#include <unordered_map>

#define STRING_BUF_LEN 65536

namespace CCE
{
	struct CCE_API StringMemory
	{	
		/// <summary>
		/// Description of the string memory buffer.
		/// </summary>
		struct StringMemDescritpion
		{
			StringMemDescritpion() = default;
			StringMemDescritpion(UINT64 _handle, UINT64 _refCount, char* _pStringBuf, UINT64 _length):
				handle(_handle), refCount(_refCount), pStringBuf(_pStringBuf), length(_length) {}
			~StringMemDescritpion() = default;

			UINT64 handle = 0;                  // 8 byte
			UINT64 refCount = 0;                // 8 byte
			char* pStringBuf = nullptr;         // 8 byte
			UINT64 length = 0;                  // 8 byte
		};

		StringMemory() = delete;
		~StringMemory() = delete;

		static char* GetValue(UINT64 stringHandle);
		static UINT64 GetLength(UINT64 stringHandle);

		static UINT64 CreateString(const char* pString);
		static void DestroyString(UINT64 stringHandle);
		static UINT64 GetStringID(const char* str);

	private:
		static StringMemDescritpion* FindStrMemDesc(UINT64 stringHandle);
		static char* DefragmentBuffer(const char* pos, size_t offset);

		static char stringBuf[STRING_BUF_LEN];
		static char* bufPtr;
		alignas(32) static std::vector<StringMemDescritpion> strHndlDesc;
	};
}
