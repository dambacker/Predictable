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
#include "scene01.h"
#include "app.h"

//-----------------------------------------------------------------------------
App::App()
//-----------------------------------------------------------------------------
{
	m_pScene = NULL;
}

//-----------------------------------------------------------------------------
App::~App()
//-----------------------------------------------------------------------------
{
	delete(m_pScene);
}

//-----------------------------------------------------------------------------
int App::Init()
//-----------------------------------------------------------------------------
{
	m_text.Init(80, 50);

	//heatmap
	m_heatmap.Init();
	glGenTextures(1, &m_textureHeatmap);
	m_heatmapTextureData = new unsigned char[m_heatmap.m_width*m_heatmap.m_height * 3];

	for (int i = 0; i < 1000; i++)
	{
		unsigned int x = rand() * m_heatmap.m_width  / RAND_MAX;
		unsigned int y = rand() * m_heatmap.m_height / RAND_MAX;

		m_heatmap.Add(x, y);
	}

	int* pInt = NULL;

	pInt = (int*)SafeMalloc(2 * sizeof(int));

	pInt[0] = 2;
	pInt[1] = 3;

	SafeFree(pInt);

	//scene
	m_pScene = new(Scene01);

	glGenVertexArrays(1, &m_vertexArrayId);
	glBindVertexArray(m_vertexArrayId);

	m_object.CreateCube(1);

	m_programId = LoadShaders("shaders/shader00.vs", "shaders/shader00.fs");
	if (m_programId == 0)
	{
		return 0;
	}
	m_matrixId  = glGetUniformLocation(m_programId, "modelViewProjection");
	m_textureId = glGetUniformLocation(m_programId, "textureSampler");

	//--- texture test ---
	unsigned int width  = 256;
	unsigned int height = 256;
	unsigned char* data = new unsigned char[width*height * 3];
	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			data[(x + y * width) * 3 + 0] = x % 0xFF;
			data[(x + y * width) * 3 + 1] = y % 0xFF;
			data[(x + y * width) * 3 + 2] = (((x/4+y/4))%2) * 0xFF;
		}
	}
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);	//let's not do this

	// Set default GL state
	glClearColor(0.0f, 0.5f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	return 1;
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
void App::Render(float time, float xpos, float ypos)
//-----------------------------------------------------------------------------
{
	float fov = 45.0;

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov), 4.0f / 3.0f, 0.1f, 100.0f);

	glm::vec3 position	= glm::vec3(0, 0, 5.0f);
	glm::vec3 direction	= glm::vec3(0, 0, 0);
	glm::vec3 up		= glm::vec3(0, 1, 0);
	glm::mat4 viewMatrix = glm::lookAt(position, direction, up);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_programId);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureHeatmap);
	glUniform1i(m_textureId, 0);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::mat4 modelMatrix;
	glm::mat4 MVP;

	modelMatrix = glm::rotate(glm::mat4(1.0), time, glm::vec3(0, 1.0, 0));
//	modelMatrix = glm::rotate(glm::mat4(1.0), 0.0f, glm::vec3(0, 1.0, 0));
	MVP = projectionMatrix * viewMatrix * modelMatrix;
	glUniformMatrix4fv(m_matrixId, 1, GL_FALSE, &MVP[0][0]);
	m_object.Draw();

	m_text.Render(1, 1, "First test of text!");
}

