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

		String(const String& other)
		{ 
			if (&other == this)
			{ return; }
			sId = other.sId;
		}

		String(String&& other) noexcept
		{ 
			if (&other == this)
			{ return; }

			sId = other.sId;
			other.sId = -1; 
		}

		String& operator=(const String& other)
		{
			// check if both refs are the same instance
			if (this == &other)
			{ return *this; }

			this->sId = other.sId;
			return *this;
		}

		String& operator=(String&& other)
		{
			// check if both refs are the same instance
			if (this == &other)
			{ return *this; }

			this->sId = other.sId;
			other.sId = -1;
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

		String operator+=(const String& other)
		{
			if (Length() == 0)
			{
				return other;
			}

			if (other.Length() == 0)
			{
				return *this;
			}

			char buf[4096];
			ZeroMemory(&buf[0], sizeof(buf));

			memcpy(&buf[0], Value(), Length());
			memcpy(&buf[0 + Length()], other.Value(), other.Length());

			sId = GetStringID(&buf[0]);
			gStringTable.try_emplace(sId, std::move(&buf[0]));
			return String(&buf[0]);
		}

		String operator+(const String& other)
		{
			return *this += other;
		}

		UINT64 Length() const;
		const char* Value() const;
		static bool IsEmpty(String& str);
		static const char* ValueBySID(UINT64 sId);

	public:
		UINT64 sId = 0;			// 8 bytes
		
	private:
		static std::unordered_map<UINT64, const char*> gStringTable;

	private:
		const UINT64 GetStringID(const char* str);
	};
}
