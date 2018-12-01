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
	int  Init();
	void Cleanup();

	void Update();
	void Render(float time);

private:
//	GLuint			m_programId;
	GLuint			m_matrixId;
	GLuint			m_textureId;
//	GLuint			m_texture;
	GLuint			m_textureHeatmap;
	unsigned char*	m_heatmapTextureData;
	GLuint			m_vertexArrayId;

	Object			m_object;
	CShader*		m_pShader;
	CTexture*		m_pTexture;
};

