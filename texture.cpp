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
#include "texture.h"

//-----------------------------------------------------------------------------
CTexture::CTexture()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
CTexture::~CTexture()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
unsigned int CTexture::LoadBmp(const char* pFilePath)
//-----------------------------------------------------------------------------
{
	unsigned char* pData = NULL;
	
	unsigned int dataSize = LoadFileToMemory(pFilePath, &pData);
	if (dataSize == 0)
	{
		printf("Can't open file %s.\n", pFilePath);
		return R_ERROR;
	}

	unsigned int width = *(int*)&pData[18];
	unsigned int height = *(int*)&pData[22];
	unsigned int size = width * height * 3;

	if (dataSize < 54 + size)
	{
		printf("BMP file size error\n");
		return R_ERROR;
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

	/* DEBUG */
	glGenerateMipmap(GL_TEXTURE_2D);	//let's not do this
	/* DEBUG */

	free(pData);

	return R_OK;
}

//-----------------------------------------------------------------------------
unsigned int CTexture::LoadKtx(const char* pFilePath)
//-----------------------------------------------------------------------------
{
	return R_OK;
}

//-----------------------------------------------------------------------------
unsigned int CTexture::Generate(unsigned int width, unsigned int height, unsigned int pattern)
//-----------------------------------------------------------------------------
{
	unsigned int	size	= width*height*4;
	unsigned char*	pData	= (unsigned char*)SafeMalloc(size);

	for (unsigned int y=0; y<height; y++)
	{
		for (unsigned int x=0; x<width; x++)
		{
			pData[(x+y*width)*4+0] = x % 0xFF;
			pData[(x+y*width)*4+1] = y % 0xFF;
			pData[(x+y*width)*4+2] = (((x/4+y/4))%2) * 0xFF;
			pData[(x+y*width)*4+3] = 0xFF;
		}
	}

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, pData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	/* DEBUG */
	glGenerateMipmap(GL_TEXTURE_2D);	//let's not do this
	/* DEBUG */

	free(pData);

	return R_OK;
}