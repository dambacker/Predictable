//
// Copyright (C) 2018, Dam Backer
//

#pragma once

#include <vector>
#include "text.h"
#include "heatmap.h"

class Scene01;

class App
{
public:
	App();
	~App();

public:
	int  Init();
	void Cleanup();

	void Update(float xpos, float ypos);
	void Render(float time, float xpos, float ypos);

private:
	GLuint			m_programId;
	GLuint			m_matrixId;
	GLuint			m_textureId;
	GLuint			m_texture;
	GLuint			m_textureHeatmap;
	unsigned char*	m_heatmapTextureData;
	GLuint			m_vertexArrayId;
	Object			m_object;

	Text			m_text;
	Heatmap			m_heatmap;

	Scene01*		m_pScene;
};

