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

//-----------------------------------------------------------------------------
Scene01::Scene01()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Scene01::~Scene01()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
int Scene01::Init()
//-----------------------------------------------------------------------------
{
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
void Scene01::Cleanup()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Scene01::Update()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Scene01::Render(float time)
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
}

