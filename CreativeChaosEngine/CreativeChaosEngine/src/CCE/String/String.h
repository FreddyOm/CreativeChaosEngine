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

		// TODO: Add copy and move constructors and move impl to cpp

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

		String operator+(const String& other)
		{
			std::string concatStr = std::string(Value()) + std::string(other.Value());
			return String(concatStr.c_str());
		}

		String operator+=(const String& other)
		{
			std::string concatStr = std::string(Value()) + std::string(other.Value());
			return String(concatStr.c_str());
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
