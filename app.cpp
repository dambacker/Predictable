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

#include "utils.h"
#include "object.h"
#include "texture.h"
#include "scene01.h"
#include "scene02.h"
#include "app.h"

//-----------------------------------------------------------------------------
App::App()
//-----------------------------------------------------------------------------
{
	m_pScene01 = NULL;
	m_pScene02 = NULL;
}

//-----------------------------------------------------------------------------
App::~App()
//-----------------------------------------------------------------------------
{
	delete(m_pScene01);
	delete(m_pScene02);
}

//-----------------------------------------------------------------------------
unsigned int App::Init()
//-----------------------------------------------------------------------------
{
	m_text.Init(80, 50);

	//heatmap
	m_heatmap.Init();
	glGenTextures(1, &m_textureHeatmap);
	m_heatmapTextureData = new unsigned char[m_heatmap.m_width*m_heatmap.m_height * 3];

	//scene01
	m_pScene01 = new(Scene01);
	if (m_pScene01->Init() != R_OK)
	{
		return R_ERROR;
	}

	//scene02
	m_pScene02 = new(Scene02);
	if (m_pScene02->Init() != R_OK)
	{
		return R_ERROR;
	}

	// Set default GL state
	glGenVertexArrays(1, &m_vertexArrayId);	//not sure why this is required
	glBindVertexArray(m_vertexArrayId);
	glClearColor(0.0f, 0.5f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	return R_OK;
}

//-----------------------------------------------------------------------------
void App::Cleanup()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void App::Update(float xpos, float ypos)
//-----------------------------------------------------------------------------
{
	unsigned int x = (unsigned int)(xpos * m_heatmap.m_width);
	unsigned int y = (unsigned int)(ypos * m_heatmap.m_height);

	assert(x < m_heatmap.m_width);
	assert(y < m_heatmap.m_height);

	m_heatmap.Add(x, y);

	//heatmap to texture
	for (unsigned int y = 0; y < m_heatmap.m_height; y++)
	{
		for (unsigned int x = 0; x < m_heatmap.m_width; x++)
		{
			unsigned int d = m_heatmap.m_pData[x][y];
			if (d > 0)
			{
				m_heatmapTextureData[(x + y * m_heatmap.m_width) * 3 + 0] = m_heatmap.m_pData[x][y] * 255 / m_heatmap.m_max;
			}
			m_heatmapTextureData[(x + y * m_heatmap.m_width) * 3 + 1] = 0x10;
			m_heatmapTextureData[(x + y * m_heatmap.m_width) * 3 + 2] = (((x / 4 + y / 4)) % 2) * 0x10;
		}
	}

	glBindTexture(GL_TEXTURE_2D, m_textureHeatmap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_heatmap.m_width, m_heatmap.m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_heatmapTextureData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

//-----------------------------------------------------------------------------
void App::Render(float time)
//-----------------------------------------------------------------------------
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pScene01->Render(time);
	m_pScene02->Render(time);

	m_text.Render(1, 1, "Predictable");
}

