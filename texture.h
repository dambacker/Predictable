//
// Copyright (C) 2018, Dam Backer
//

#pragma once

class CTexture
{
public:
	CTexture();
	~CTexture();

public:
	unsigned int	Generate(unsigned int width, unsigned int height, unsigned int pattern);
	unsigned int	LoadBmp(char const* pFilePath);
	unsigned int	LoadKtx(char const* pFilePath);

public:
	GLuint	m_texture;
};

