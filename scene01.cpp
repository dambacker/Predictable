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
#include "shader.h"
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

	m_pShader = new(CShader);
	m_pShader->LoadFromFile("shaders/shader00.vs", "shaders/shader00.fs");
	m_matrixId  = glGetUniformLocation(m_pShader->m_program, "modelViewProjection");
	m_textureId = glGetUniformLocation(m_pShader->m_program, "textureSampler");

	m_pTexture = new(CTexture);
	m_pTexture->Generate(256, 256, 0);

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

	glUseProgram(m_pShader->m_program);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pTexture->m_texture);
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

