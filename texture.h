//
// Copyright (C) 2018, Dam Backer
//

#pragma once

class CTexture
{
public:
	typedef enum
	{
		BASIC1,
		BASIC2,
		GAUSSIAN,
	} Pattern;

public:
	CTexture();
	~CTexture();

public:
	unsigned int	Generate(unsigned int width, unsigned int height, Pattern pattern);
	unsigned int	LoadBmp(char const* pFilePath);
	unsigned int	LoadAstc(unsigned int width, unsigned int height, unsigned int level, const char* pFilePath);
	unsigned int	LoadKtx(char const* pFilePath);

public:
	GLuint	m_texture;
};

