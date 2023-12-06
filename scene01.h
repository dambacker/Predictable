//
// Copyright (C) 2018, Dam Backer
//

#pragma once

#include <vector>
#include "text.h"
#include "heatmap.h"

class CShader;
class CTexture;

class Scene01
{
public:
	Scene01();
	~Scene01();

public:
	unsigned int	Init(float aspectX, float aspectY);
	void			Cleanup();

	void			Update();
	void			Render(float time);

private:
	float			m_aspectX;
	float			m_aspectY;
	Object			m_object;
	CShader*		m_pShader;
	GLuint			m_matrixId;
	GLuint			m_textureId;
	CTexture*		m_pTexture;
};

