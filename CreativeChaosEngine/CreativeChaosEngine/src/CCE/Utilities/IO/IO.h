#pragma once
#include "../../Core.h"
#include "../../String/String.h"
#include "../../Analysis/Debug.h"
#include <iostream>
#include <fstream>
#include <string>

namespace CCE
{
	class CCE_API IO
	{
		enum class FileMode
		{
			DEFAULT = 0x00, // default
			END_OF_FILE = 0x04, //std::ios::ate
			BINARY = 0x20, //std::ios::binary
			APPEND = 0x08, //std::ios::app
			OVERRIDE = 0x10, //std::ios::trunc
		};

	public:

		static CCE::String ReadText(CCE::String filePath, FileMode fileMode = FileMode::DEFAULT);
		//static std::shared_ptr<char> ReadBytes(CCE::String filePath, FileMode fileMode = FileMode::DEFAULT);

		static bool WriteText(CCE::String filePath, CCE::String input, FileMode fileMode = FileMode::DEFAULT);
		static bool WriteBytes(CCE::String filePath, char* input, size_t size, FileMode fileMod = FileMode::DEFAULT);
	};
}