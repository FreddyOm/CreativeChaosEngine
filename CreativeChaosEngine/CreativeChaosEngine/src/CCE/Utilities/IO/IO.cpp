#include "IO.h"

namespace CCE
{

	/// <summary>
	/// Checks if a specific file exists or not.
	/// </summary>
	/// <param name="filePath">The path to the file.</param>
	/// <returns></returns>
	bool File::Exists(String filePath)
	{
		if (FILE* file = fopen(filePath.Value(), "r")) {
			fclose(file);
			return true;
		}
		else {
			return false;
		}
	}

	/// <summary>
	/// Checks if a specific file exists or not.
	/// </summary>
	/// <param name="filePath">The file.</param>
	/// <returns></returns>
	bool File::Exists(File file)
	{
		if (FILE* _file = fopen(file.GetPath().Value(), "r")) {
			fclose(_file);
			return true;
		}
		else {
			return false;
		}
	}

	File File::Create(String filePath)
	{
		if (!File::Exists(filePath))
		{
			fopen(filePath.Value(), "w+");
		}

		return File(filePath);
	}

	String IO::ReadText(const String& filePath, const FileMode fileMode)
	{
		// TODO: Append the flags according to the fileMode input(s)
		//DASSERT(!String::IsEmpty(filePath), "The filepath must not be empty!");
		
		if (!File::Exists(filePath)) { return CCE::String(""); }

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

	CCE::String IO::ReadText(const File& file, const FileMode fileMode)
	{
		return ReadText(file.GetPath(), fileMode);
	}

	/*
	std::shared_ptr<char> IO::ReadBytes(String& filePath, FileMode fileMode)
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
	
	bool IO::WriteText(const String& filePath, const String input, bool createFileIfNonExistent, FileMode fileMode)
	{
		if (!File::Exists(filePath))
		{
			if (createFileIfNonExistent)
				File::Create(filePath);
			else
				return false;
		}

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

	bool IO::WriteText(const File& file, const String input, bool createFileIfNonExistent, FileMode fileMode)
	{
		return WriteText(file.GetPath(), input, createFileIfNonExistent, fileMode);
	}

	bool IO::WriteBytes(const String& filePath, const char* input, size_t size, bool createFileIfNonExistent, FileMode fileMod)
	{
		//DASSERT(!String::IsEmpty(filePath), "The filepath must not be empty!");
		return false;
	}

}