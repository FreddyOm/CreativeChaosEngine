#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "../../Core.h"
#include "../../String/String.h"
#include "../../Analysis/Debug.h"
#include "../../String/StringConverter.h"

namespace CCE
{
#pragma warning(disable : 4996)

	class CCE_API File
	{
	public:

		File(const String& filePath)
			: filePath(filePath)
		{
			if(!Exists(filePath))
				DWARNING("File was not found. Create it or check the file path!");
		}

		File()
			: filePath("")
		{ }

		~File() = default;

		File(const File& other)
			: filePath(other.filePath)
		{
			if(&other == this)
			{ return; }
			filePath = other.filePath;
		}

		File(File&& other) noexcept
		{ 
			if (&other == this)
			{ return; }
			filePath = other.filePath;
			other.filePath = ""; 
		}

		static bool Exists(const String& filePath);
		static File Create(const String& filePath);

		String Path() const
		{
			return filePath;
		}

		std::string GetPathSTDString() const
		{
			return filePath.Value();
		}

		std::wstring GetPathWSTDString() const
		{
			return StringConverter::StringToWString(filePath.Value());
		}

		File& operator=(const File& other)
		{
			if (this == &other)
			{ return *this; }
			this->filePath = other.filePath;
			return *this;
		}

		File& operator=(File&& other)
		{
			if (this == &other)
			{ return *this; }
			this->filePath = other.filePath;
			other.filePath = "";
			return *this;
		}

	private:
		String filePath;
	};

	class CCE_API Directory
	{
	public:

		Directory(const String& dirPath)
			: dirPath(dirPath)
		{ }

		Directory()
			: dirPath("")
		{ }

		Directory(const Directory& other)
		{
			if(&other == this)
			{ return; }
			dirPath = other.dirPath;
		}

		Directory(Directory&& other) noexcept
		{
			if (&other == this)
			{ return; }
			dirPath = other.dirPath;
			other.dirPath = "";
		}

		~Directory() = default;

		static bool Exists(const String& dirPath);
		static bool IsEmpty(const String& dirPath);

		static Directory Create(const String& dirPath);

		String Path() const
		{
			return dirPath;
		}

		Directory& operator=(const Directory& other)
		{
			if (&other == this)
			{ return *this; }
			dirPath = other.dirPath;
			return *this;
		}

		Directory& operator=(Directory&& other)
		{
			if (&other == this)
			{ return *this; }
			dirPath = other.dirPath;
			other.dirPath = "";
			return *this;
		}

	private:
		String dirPath;
	};

	class CCE_API IO
	{
	public:

		enum class FileMode
		{
			DEFAULT = 0x00, // default
			END_OF_FILE = 0x04, //std::ios::ate
			BINARY = 0x20, //std::ios::binary
			APPEND = 0x08, //std::ios::app
			OVERRIDE = 0x10, //std::ios::trunc
		};

		static CCE::String ReadText(const String filePath, const FileMode fileMode = FileMode::DEFAULT);
		static CCE::String ReadText(const File file, const FileMode fileMode = FileMode::DEFAULT);
		// TODO: Implement Reading bytes
		//static std::shared_ptr<char> ReadBytes(String filePath, FileMode fileMode = FileMode::DEFAULT);

		static bool WriteText(const String filePath, const String input, bool createFileIfNonExistent = false, FileMode fileMode = FileMode::DEFAULT);
		static bool WriteText(const File file, const String input, bool createFileIfNonExistent = false, FileMode fileMode = FileMode::DEFAULT);
		static bool WriteBytes(const String filePath, const char* input, size_t size, bool createFileIfNonExistent = false, FileMode fileMod = FileMode::DEFAULT);
	};
}