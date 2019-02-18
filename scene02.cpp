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
#include "scene02.h"

//-----------------------------------------------------------------------------
Scene02::Scene02()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Scene02::~Scene02()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
unsigned int Scene02::Init()
//-----------------------------------------------------------------------------
{
	m_object.CreateQuad();

	m_pShader = new(CShader);
	m_pShader->LoadFromFile("shaders/blit2D.vs", "shaders/blit2D.fs");
	m_matrixId  = glGetUniformLocation(m_pShader->m_program, "modelViewProjection");
	m_textureId = glGetUniformLocation(m_pShader->m_program, "textureSampler");

	m_pTexture = new(CTexture);
	m_pTexture->Generate(256, 256, CTexture::GAUSSIAN);

	return R_OK;
}

//-----------------------------------------------------------------------------
void Scene02::Cleanup()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Scene02::Update()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Scene02::Render(float time)
//-----------------------------------------------------------------------------
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(m_pShader->m_program);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pTexture->m_texture);
	glUniform1i(m_textureId, 0);

	glm::mat4 MVP = glm::mat4(1.0);
	MVP = glm::translate(MVP, glm::vec3(0, 0.1, 0));
	MVP = glm::scale(MVP, glm::vec3(0.5, 0.5, 0));
	glUniformMatrix4fv(m_matrixId, 1, GL_FALSE, &MVP[0][0]);

	m_object.Draw();
}

