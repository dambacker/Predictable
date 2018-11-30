//
// Copyright (C) 2018, Dam Backer
//

#pragma once

class CTextures
{
public:
	CTextures();
	~CTextures();

public:
	GLuint	LoadBmp(char const* pFilePath);
	GLuint	LoadKtx(char const* pFilePath);

protected:
	GLuint	m_texture;
};

