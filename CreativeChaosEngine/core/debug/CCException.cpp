#include "CCException.h"
#include <sstream>

namespace CCE_Debug
{

	CCException::CCException(int line, const char* file) noexcept
		: line(line), file(file)
	{

	}

	const char* CCException::what() const noexcept
	{
		std::ostringstream oss;

		oss << "CCException" << GetOriginString();
		whatBuffer = oss.str();

		return whatBuffer.c_str();
	}

	int CCException::GetLine() const noexcept
	{
		return line;
	}

	const std::string& CCException::GetFile() const noexcept
	{
		return file;
	}

	std::string CCException::GetOriginString() const noexcept
	{
		std::ostringstream oss;
		oss << "[FILE]: " << file << std::endl
			<< "[LINE]: " << line;
		return oss.str();
	}

}