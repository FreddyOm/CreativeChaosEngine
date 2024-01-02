#pragma once
#include "../Core.h"
#include "StringMemory.h"
#include "../Analysis/Debug.h"

namespace CCE
{
	struct CCE_API String // 8 bytes
	{
		String(const char* str = "");
		~String();

		String(const String& other)
		{ 
			if (&other == this)
			{ return; }
			sId = StringMemory::CreateString(other.Value());
		}

		String(String&& other) noexcept
		{ 
			if (&other == this)
			{ return; }

			sId = StringMemory::CreateString(other.Value());
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

		String& operator=(String&& other) noexcept
		{
			// check if both refs are the same instance
			if (this == &other)
			{ return *this; }

			this->sId = StringMemory::CreateString(other.Value());;
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
			return sId == StringMemory::GetStringID(other);
		}

		bool operator!=(const char* other)
		{
			return sId != StringMemory::GetStringID(other);
		}

		String operator+=(const String& other)
		{
			DASSERT(Length() + other.Length() < 4096,
				"Buffer overflow on String concatination!");

			char buf[4096];
			ZeroMemory(&buf[0], sizeof(buf));
			memcpy(&buf[0], this->Value(), this->Length());
			memcpy(&buf[this->Length()], other.Value(), other.Length());

			return String(&buf[0]);
		}

		String operator+(const String& other)
		{
			DASSERT(Length() + other.Length() < 4096,
				"Buffer overflow on String concatination!");

			char buf[4096];
			ZeroMemory(&buf[0], sizeof(buf));
			memcpy(&buf[0], this->Value(), this->Length());
			memcpy(&buf[this->Length()], other.Value(), other.Length());

			return String(&buf[0]);
		}

		bool operator<(const String& other)
		{
			return this->sId < other.sId;
		}

		UINT64 Length() const;
		const char* Value() const;
		static bool IsEmpty(String& str);
		static const char* ValueBySID(UINT64 sId);

	public:
		UINT64 sId = 0;			// 8 bytes
	};
}
