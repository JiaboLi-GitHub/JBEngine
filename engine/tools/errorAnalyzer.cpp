#include "errorAnalyzer.h"
#include <iostream>
#include "../global/constant.h"

namespace JB
{
	void ErrorAnalyzer::checkErrors()
	{
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			switch (err) {
			case GL_INVALID_OPERATION:
				printf("Invalid operation\n");
				break;
			case GL_INVALID_VALUE:
				printf("Invalid value\n");
				break;
			case GL_INVALID_ENUM:
				printf("Invalid enum\n");
				break;
			case GL_OUT_OF_MEMORY:
				printf("Out of memory\n");
				break;
			default:
				printf("Unknown error\n");
				break;
			}
		}
	}

	void ErrorAnalyzer::checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
}
