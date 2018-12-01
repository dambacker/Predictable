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
	unsigned int	Init();
	void			Cleanup();

	void			Update();
	void			Render(float time);

private:
	Object			m_object;
	CShader*		m_pShader;
	GLuint			m_matrixId;
	GLuint			m_textureId;
	CTexture*		m_pTexture;
};

