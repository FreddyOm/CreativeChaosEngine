#pragma once
#include <exception>
#include <string>

namespace CCE_Debug
{

	class CCException : public std::exception
	{
	public:
		CCException(int line, const char* file) noexcept;
		const char* what() const noexcept override;
		int GetLine() const noexcept;
		const std::string& GetFile() const noexcept;
		std::string GetOriginString() const noexcept;
	private:
		int line;
		std::string file;
	protected:
		mutable std::string whatBuffer;
	};

#define DEFAULT_EXCEPT()(throw CCException(__LINE__,__FILE__))

}