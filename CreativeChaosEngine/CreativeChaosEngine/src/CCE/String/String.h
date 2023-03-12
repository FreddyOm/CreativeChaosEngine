#pragma once
#include "../Core.h"
#include <unordered_map>
#include <iterator>
#include "../Analysis/Debug.h"

namespace CCE
{
	struct CCE_API String // 8 bytes
	{
		String(const char* str = "");
		~String();

		void ClearGlobalStringTable();

		String& operator=(const String& other)
		{
			// check if both refs are the same instance
			if (this == &other)
			{
				return *this;
			}

			this->sId = other.sId;
			return *this;
		}

		String& operator=(const char* other)
		{
			*this = String(other);
			return *this;
		}

		bool operator==(const String& other)
		{
			// compare strings by their string id
			return this->sId == other.sId;
		}

		bool operator!=(const String& other)
		{
			// compare strings by their string id
			return this->sId != other.sId;
		}

		bool operator==(const char* other)
		{
			return this->sId == GetStringID(other);
		}

		bool operator!=(const char* other)
		{
			return this->sId != GetStringID(other);
		}

		size_t Length() const;
		const char* Value() const;

	public:
		unsigned long long sId = 0;			// 8 bytes
		
	private:
		static std::unordered_map<unsigned long long, const char*> gStringTable;

	private:
		const unsigned long long GetStringID(const char* str);
	};
}
