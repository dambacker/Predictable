//
// Copyright (C) 2018, Dam Backer
//

#include "pch.h"

#include <windows.h>
#include <fstream>
#include <sstream>
#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "utils.h"
#include "shader.h"

//-----------------------------------------------------------------------------
CShader::CShader()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
CShader::~CShader()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
GLuint CShader::LoadFromFile(const char* pVertexFilePath, const char* pFragmentFilePath)
//-----------------------------------------------------------------------------
{
	// Create the shaders
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertexShaderCode;
	std::string fragmentShaderCode;

	if (LoadFileToString(pVertexFilePath, vertexShaderCode) == 0)
	{
		printf("Can't open file %s.\n", pVertexFilePath);
		return R_ERROR;
	}

	if (LoadFileToString(pFragmentFilePath, fragmentShaderCode) == 0)
	{
		printf("Can't open file %s.\n", pFragmentFilePath);
		return R_ERROR;
	}

	GLint result = GL_FALSE;
	int infoLogLength = 0;

	// Compile Vertex Shader
	char const* pVertexSourcePointer = vertexShaderCode.c_str();
	glShaderSource(vertexShaderID, 1, &pVertexSourcePointer, NULL);
	glCompileShader(vertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::vector<char> VertexShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
		return R_ERROR;
	}

	// Compile Fragment Shader
	char const* pFragmentSourcePointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID, 1, &pFragmentSourcePointer, NULL);
	glCompileShader(fragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::vector<char> FragmentShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
		return R_ERROR;
	}

	// Link the program
	m_program = glCreateProgram();
	glAttachShader(m_program, vertexShaderID);
	glAttachShader(m_program, fragmentShaderID);
	glLinkProgram(m_program);

	// Check the program
	glGetProgramiv(m_program, GL_LINK_STATUS, &result);
	glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::vector<char> ProgramErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(m_program, infoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
		return R_ERROR;
	}

	glDetachShader(m_program, vertexShaderID);
	glDetachShader(m_program, fragmentShaderID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return R_OK;
}
