//
// Copyright (C) 2018-2019, Dam Backer
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
unsigned int Scene01::Init(float aspectX, float aspectY)
//-----------------------------------------------------------------------------
{
    m_aspectX = aspectX;
    m_aspectY = aspectY;

    m_object.CreateQuad();

    m_pShader = new(CShader);
    m_pShader->LoadFromFile("shaders/blit2D.vs", "shaders/blit2D.fs");
    m_matrixId  = glGetUniformLocation(m_pShader->m_program, "modelViewProjection");
    m_textureId = glGetUniformLocation(m_pShader->m_program, "textureSampler");

    m_pTexture = new(CTexture);
    m_pTexture->LoadKtx("textures/image_painting.ktx");

    return R_OK;
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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(m_pShader->m_program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_pTexture->m_texture);
    glUniform1i(m_textureId, 0);

    glm::mat4 mvp = glm::mat4(1.0);
//  mvp = glm::translate(mvp, glm::vec3(0, 0.1, 0));
    mvp = glm::scale(mvp, glm::vec3(0.95f * m_aspectX, 0.95f * m_aspectY, 0));
    glUniformMatrix4fv(m_matrixId, 1, GL_FALSE, &mvp[0][0]);

    m_object.Draw();
}

