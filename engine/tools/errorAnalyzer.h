#pragma once
#include <string>

namespace JB
{
	class ErrorAnalyzer
	{
	public:
		static void checkErrors();
		static void checkCompileErrors(unsigned int shader, std::string type);
	};
}

