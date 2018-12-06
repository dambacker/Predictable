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

#include <ktx.h>

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
unsigned int CTexture::Generate(unsigned int width, unsigned int height, unsigned int pattern)
//-----------------------------------------------------------------------------
{
	unsigned int	size	= width*height*4;
	unsigned char*	pData	= (unsigned char*)SafeMalloc(size);

	switch (pattern)
	{
	default:
	case 0:
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
		break;
	case 1:
		for (unsigned int y=0; y<height; y++)
		{
			for (unsigned int x=0; x<width; x++)
			{
				pData[(x+y*width)*4+0] = (x*0xFF/width);
				pData[(x+y*width)*4+1] = (y*0xFF/height);
				pData[(x+y*width)*4+2] = (((x/8+y/8))%2) * 0xFF;
				pData[(x+y*width)*4+3] = 0x80;
			}
		}
		break;
	}

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData);
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
unsigned int CTexture::LoadAstc(unsigned int width, unsigned int height, unsigned int level, const char* pFilePath)
//-----------------------------------------------------------------------------
{
	unsigned char* pData = NULL;

	unsigned int dataSize = LoadFileToMemory(pFilePath, &pData);
	if (dataSize == 0)
	{
		printf("Can't open file %s.\n", pFilePath);
		return R_ERROR;
	}

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
//	glCompressedTexImage2D(GL_TEXTURE_2D, level, GL_COMPRESSED_RGBA_ASTC_4x4, width, height, 0, dataSize, pData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	free(pData);

	return R_OK;
}

//-----------------------------------------------------------------------------
unsigned int CTexture::LoadKtx(const char* pFilePath)
//-----------------------------------------------------------------------------
{
	GLenum			target		= GL_TEXTURE_2D;
	KTX_dimensions	dimensions	= {0, 0, 0};
	GLboolean		mipmapped	= GL_FALSE;
	GLenum			glerror		= 0;
	
	glGenTextures(1, &m_texture);
	if (ktxLoadTextureN(pFilePath, &m_texture, &target, &dimensions, &mipmapped, &glerror, NULL, NULL) != KTX_SUCCESS)
	{
		return R_ERROR;
	}

	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (mipmapped)
	{
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	return R_OK;
}

