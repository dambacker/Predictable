//
// Copyright (C) 2018, Dam Backer
//

#pragma once

#include <vector>
#include "mouse.h"
#include "text.h"
#include "heatmap.h"

class Scene01;
class Scene02;

class App
{
public:
	App();
	~App();

public:
	unsigned int	Init();
	void			Cleanup();

	void			Mouse(float mouseX, float mouseY, int mouseButton1);
	void			Update(float xpos, float ypos);
	void			Render(float time);

private:
	Heatmap			m_heatmap;
	GLuint			m_textureHeatmap;
	unsigned char*	m_heatmapTextureData;

	GLuint			m_vertexArrayId;

	float			m_mouseX;
	float			m_mouseY;
	int				m_mouseButton1;

	CMouse			m_mouse;
	Text			m_text;

	Scene01*		m_pScene01;
	Scene02*		m_pScene02;
};

