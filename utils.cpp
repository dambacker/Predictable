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

//-----------------------------------------------------------------------------
void*	SafeMalloc(size_t size)
//-----------------------------------------------------------------------------
{
	void* pointer = malloc(size);

	assert(pointer != NULL);

	return pointer;
}

//-----------------------------------------------------------------------------
void	_SafeFree(void* ptr)
//-----------------------------------------------------------------------------
{
	if (ptr != NULL)
	{
		free(ptr);
	}

	ptr = NULL;
}

//-----------------------------------------------------------------------------
int LoadFileToString(const char* pFilePath, std::string &str)
//-----------------------------------------------------------------------------
{
	std::ifstream stream(pFilePath, std::ios::in);
	if (!stream.is_open())
	{
		return 0;
	}
	std::stringstream stringstream;
	stringstream << stream.rdbuf();
	stream.close();

	str = stringstream.str();

	return (int)str.length();
}

//-----------------------------------------------------------------------------
int LoadFileToMemory(char const* pFilePath, unsigned char** ppData)
//-----------------------------------------------------------------------------
{
	FILE* file = NULL;

	errno_t err = fopen_s(&file, pFilePath, "rb");
	if ((err!=0) || (file == NULL))
	{
		return 0;
	}

	fseek(file, 0, SEEK_END);
	unsigned int size = ftell(file);
	fseek(file, 0, SEEK_SET);

	*ppData = (unsigned char*)malloc(size);
	if (*ppData == NULL)
	{
		fclose(file);
		return 0;
	}

	size_t s = fread(*ppData, sizeof(char), size, file);
	if (s != size)
	{
		free(*ppData);
		fclose(file);
		return 0;
	}

	fclose(file);
	return size;
}

//-----------------------------------------------------------------------------
GLuint LoadShaders(const char* pVertexFilePath, const char* pFragmentFilePath)
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
		return 0;
	}

	if (LoadFileToString(pFragmentFilePath, fragmentShaderCode) == 0)
	{
		printf("Can't open file %s.\n", pFragmentFilePath);
		return 0;
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
		return 0;
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
		return 0;
	}

	// Link the program
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::vector<char> ProgramErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
		return 0;
	}

	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return programID;
}

//-----------------------------------------------------------------------------
GLuint LoadTexture(const char* pFilePath)
//-----------------------------------------------------------------------------
{
	unsigned char* pData = NULL;
	
	unsigned int dataSize = LoadFileToMemory(pFilePath, &pData);
	if (dataSize == 0)
	{
		printf("Can't open file %s.\n", pFilePath);
		return 0;
	}

	unsigned int width = *(int*)&pData[18];
	unsigned int height = *(int*)&pData[22];
	unsigned int size = width * height * 3;

	if (dataSize < 54 + size)
	{
		printf("BMP file size error\n");
		return 0;
	}

	//swizzle R,G,B
	for (unsigned int i = 0; i < size; i += 3)
	{
		unsigned char tmp = pData[54 + i];
		pData[54 + i] = pData[54 + i + 2];
		pData[54 + i + 2] = tmp;
	}

//	//flip Y
//	unsigned char* pTmp = (unsigned char*)malloc(width * 3);
//	for (unsigned int y = 0; y < height/2; y++)
//	{
//		memcpy(pTmp, &pData[54 + (y+0)*width*3], width*3);
//		memcpy(pTmp, &pData[54 + (y+1)*width*3], width*3);
//	}

	GLuint	texture = 0;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, &pData[54]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);	//let's not do this

	free(pData);

	return texture;
}


