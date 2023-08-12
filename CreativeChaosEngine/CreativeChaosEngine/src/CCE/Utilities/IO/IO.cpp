#include "IO.h"
#include <shlwapi.h>

namespace CCE
{
	//TODO: Make this concurrently accessible
	//TODO: Fix file mode to work properly!
	//TODO: Add impl for read/write bytes
	//TODO: Impl Directory.IsEmpty(String dirPath)

	/// <summary>
	/// Checks if a specific file exists or not.
	/// </summary>
	/// <param name="filePath">The path to the file.</param>
	/// <returns></returns>
	bool File::Exists(String filePath)
	{
#ifdef CCE_PLATFORM_WINDOWS
		if (FILE* file = fopen(filePath.Value(), "r")) {
			fclose(file);
			return true;
		}
		else {
			return false;
		}
#elif 
#error CCE is currently only supported for Windows
#endif
	}

	/// <summary>
	/// Checks if a specific file exists or not.
	/// </summary>
	/// <param name="filePath">The file.</param>
	/// <returns></returns>
	bool File::Exists(File file)
	{
		return Exists(file.GetPath());
	}

	/// <summary>
	/// Creates a file at the specified path if not already existent.
	/// </summary>
	/// <param name="filePath">The path to the file to create.</param>
	/// <returns>The created file.</returns>
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
	
	/// <summary>
	/// Writes text to a specified file.
	/// </summary>
	/// <param name="filePath">The path to the file to write to.</param>
	/// <param name="input">The text to write.</param>
	/// <param name="createFileIfNonExistent">Whether or not the file should be created if not already existent.</param>
	/// <param name="fileMode">The filemode to open the file with.</param>
	/// <returns>True if successful. False if unsuccessful.</returns>
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

	/// <summary>
	/// Writes text to a specified file.
	/// </summary>
	/// <param name="file">The file to write to.</param>
	/// <param name="input">The text to write.</param>
	/// <param name="createFileIfNonExistent">Whether or not the file should be created if not already existent.</param>
	/// <param name="fileMode">The filemode to open the file with.</param>
	/// <returns>True if successful. False if unsuccessful.</returns>
	bool IO::WriteText(const File& file, const String input, bool createFileIfNonExistent, FileMode fileMode)
	{
		return WriteText(file.GetPath(), input, createFileIfNonExistent, fileMode);
	}

	/// <summary>
	/// Writes data in binary format to a specified output file.
	/// </summary>
	/// <param name="filePath">The path to the file to write to.</param>
	/// <param name="input">The data to write.</param>
	/// <param name="size">The size of the data to write.</param>
	/// <param name="createFileIfNonExistent">Whether or not to create the file if not already existent.</param>
	/// <param name="fileMode">Mode with which the file is accessed.</param>
	/// <returns>True if successful. False if unsuccessful.</returns>
	bool IO::WriteBytes(const String& filePath, const char* input, size_t size, bool createFileIfNonExistent, FileMode fileMode)
	{
		//DASSERT(!String::IsEmpty(filePath), "The filepath must not be empty!");
		return false;
	}

	/// <summary>
	/// Checks whether or not a directory exists in the file system.
	/// </summary>
	/// <param name="dirPath">The path to the directory to check.</param>
	/// <returns>True if the directory does exist. False if not.</returns>
	bool Directory::Exists(String dirPath)
	{
#ifdef CCE_PLATFORM_WINDOWS
		struct stat pathInfo;
		if (stat(dirPath.Value(), &pathInfo) == 0)
		{
			if (pathInfo.st_mode & S_IFDIR)
			{
				return true;
			}
		}
		return false;
#elif
#error CCE is currently only supported for Windows
#endif
	}

	/// <summary>
	/// Checks whether or not a directory exists in the file system.
	/// </summary>
	/// <param name="directory">The directory to check.</param>
	/// <returns>True if the directory does exist. False if not.</returns>
	bool Directory::Exists(Directory directory)
	{
		return Exists(directory.GetPath());
	}

	/// <summary>
	/// Checks whether or not a directory contains any files or subdirectories.
	/// </summary>
	/// <param name="dirPath">The path to the specified directory.</param>
	/// <returns>True if no files or subdirectories are present. 
	/// False if files or subdirectories are peresent or the directory does not exist.</returns>
	bool Directory::IsEmpty(String dirPath)
	{
#ifdef CCE_PLATFORM_WINDOWS
		//return Exists(dirPath) && PathIsDirectoryEmptyA(dirPath.Value());
		throw E_NOTIMPL;
#elif
#error CCE is currently only supported for Windows
#endif
	}

	/// <summary>
	/// Checks whether or not a directory contains any files or subdirectories.
	/// </summary>
	/// <param name="directory">The path to the specified directory.</param>
	/// <returns>True if no files or subdirectories are present. 
	/// False if files or subdirectories are peresent or the directory does not exist.</returns>
	bool Directory::IsEmpty(Directory directory)
	{
		return IsEmpty(directory.GetPath());
	}

	/// <summary>
	/// Creates a directory at the specified path. 
	/// If unsuccessful an error is thrown and an empty directory is returned.
	/// </summary>
	/// <param name="dirPath">The path to create the directory in.</param>
	/// <returns>The directory with the specified path.</returns>
	Directory Directory::Create(String dirPath)
	{
#ifdef CCE_PLATFORM_WINDOWS
		if (Exists(dirPath)) { return Directory(dirPath); }

		if (SUCCEEDED(CreateDirectoryA(dirPath.Value(), NULL)))
		{ return Directory(dirPath); }
		else
		{ DERROR("Failed creating a directory at path \"%s\".", dirPath.Value()); }
#elif
#error CCE is currently only supported for Windows
#endif
		return Directory("");
	}
}