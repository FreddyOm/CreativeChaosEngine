#include "IO.h"

namespace CCE
{
	String IO::ReadText(String filePath, FileMode fileMode)
	{
		// TODO: Append the flags according to the fileMode input(s)
		DASSERT(!String::IsEmpty(filePath), "The filepath must not be empty!");
		
		std::string line;
		char buf[4096];
		std::ifstream openFileStream(filePath.Value());

		if (openFileStream.is_open())
		{
			unsigned int textEnd = 0;
			while (std::getline(openFileStream, line))
			{
				for (int i = 0; i < line.length(); ++i)
				{
					buf[textEnd] = line.at(i);
					textEnd++;
				}
			}
			buf[textEnd] = 0;
		}
		else 
		{
			DERROR("Failed to read file \"%s\"", filePath);
		}
		
		openFileStream.close();

		return CCE::String(&buf[0]);
	}

	/*
	std::shared_ptr<char> IO::ReadBytes(String filePath, FileMode fileMode)
	{
		DASSERT(!String::IsEmpty(filePath), "The filepath must not be empty!");
		std::shared_ptr<char> output = nullptr;

		std::streampos size;
		char* memblock;

		std::ifstream openFileStream(filePath.Value(), std::ios::in | std::ios::binary);
		if (openFileStream.is_open())
		{
			size = openFileStream.tellg();
			memblock = new char[size];
			openFileStream.seekg(0, std::ios::beg);
			openFileStream.read(memblock, size);
			openFileStream.close();
		}

		//TODO: Check if this is a memory leak (memblock might not be deleted here!)
		output = std::make_shared<char>(memblock);

		return output;
	}
	*/
	

	bool IO::WriteText(String filePath, String input, FileMode fileMode)
	{
		DASSERT(!String::IsEmpty(filePath), "The filepath must not be empty!");

		std::ofstream openFileStream(filePath.Value());
		if (openFileStream.is_open())
		{
			unsigned int index = 0;
			openFileStream << input.Value();
			openFileStream.close();

			return true;
		}

		return false;
	}

	bool IO::WriteBytes(String filePath, char* input, size_t size, FileMode fileMod)
	{
		DASSERT(!String::IsEmpty(filePath), "The filepath must not be empty!");
		return false;
	}
}