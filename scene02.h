//
// Copyright (C) 2018, Dam Backer
//

#pragma once

#include <vector>
#include "text.h"
#include "heatmap.h"

class CShader;
class CTexture;

class Scene02
{
public:
	Scene02();
	~Scene02();

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

