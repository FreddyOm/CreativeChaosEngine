#pragma once
#include "../../Core.h"
#include "../../String/String.h"
#include "../../Analysis/Debug.h"
#include <iostream>
#include <fstream>
#include <string>

namespace CCE
{
#pragma warning(disable : 4996)

	class CCE_API File
	{
	public:

		File(String filePath)
			: filePath(filePath)
		{ }

		File()
			: filePath("")
		{ }

		static bool Exists(String filePath);
		static bool Exists(File file);

		static File Create(String filePath);

		String GetPath() const
		{
			return filePath;
		}

	private:

		String filePath;
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

		static CCE::String ReadText(const String& filePath, const FileMode fileMode = FileMode::DEFAULT);
		static CCE::String ReadText(const File& file, const FileMode fileMode = FileMode::DEFAULT);
		//static std::shared_ptr<char> ReadBytes(String filePath, FileMode fileMode = FileMode::DEFAULT);

		static bool WriteText(const String& filePath, const String input, bool createFileIfNonExistent = false, FileMode fileMode = FileMode::DEFAULT);
		static bool WriteText(const File& file, const String input, bool createFileIfNonExistent = false, FileMode fileMode = FileMode::DEFAULT);
		static bool WriteBytes(const String& filePath, const char* input, size_t size, bool createFileIfNonExistent = false, FileMode fileMod = FileMode::DEFAULT);
	};
}