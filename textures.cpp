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
#include "textures.h"

//-----------------------------------------------------------------------------
CTextures::CTextures()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
CTextures::~CTextures()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
GLuint CTextures::LoadBmp(const char* pFilePath)
//-----------------------------------------------------------------------------
{
	unsigned char* pData = NULL;
	
	unsigned int dataSize = LoadFileToMemory(pFilePath, &pData);
	if (dataSize == 0)
	{
		printf("Can't open file %s.\n", pFilePath);
		return 1;
	}

	unsigned int width = *(int*)&pData[18];
	unsigned int height = *(int*)&pData[22];
	unsigned int size = width * height * 3;

	if (dataSize < 54 + size)
	{
		printf("BMP file size error\n");
		return 1;
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

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, &pData[54]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);	//let's not do this

	free(pData);

	return 0;
}

//-----------------------------------------------------------------------------
GLuint CTextures::LoadKtx(const char* pFilePath)
//-----------------------------------------------------------------------------
{
	return 0;
}
