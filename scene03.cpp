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
#include "scene03.h"

//-----------------------------------------------------------------------------
Scene03::Scene03()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Scene03::~Scene03()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
unsigned int Scene03::Init()
//-----------------------------------------------------------------------------
{
    m_object.CreateCube2(1);

    m_pShader = new(CShader);
    m_pShader->LoadFromFile("shaders/shader00.vs", "shaders/shader00.fs");
    m_matrixId  = glGetUniformLocation(m_pShader->m_program, "modelViewProjection");
    m_textureId = glGetUniformLocation(m_pShader->m_program, "textureSampler");

    m_pTexture = new(CTexture);
    m_pTexture->LoadKtx("textures/image_painting.ktx");

    return R_OK;
}

//-----------------------------------------------------------------------------
void Scene03::Cleanup()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Scene03::Update()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Scene03::Render(float time, GLuint textureId)
//-----------------------------------------------------------------------------
{
    using namespace glm;

    float fov = 45.0;

    mat4 projectionMatrix = perspective(radians(fov), 4.0f / 3.0f, 0.1f, 100.0f);

    vec3 position   = vec3(0, 0, 5.0f);
    vec3 direction  = vec3(0, 0, 0);
    vec3 up         = vec3(0, 1, 0);
    mat4 viewMatrix = lookAt(position, direction, up);

    glUseProgram(m_pShader->m_program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_pTexture->m_texture); //textureId
    glUniform1i(m_textureId, 0);

    glDisable(GL_CULL_FACE);    //grrr
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mat4 modelMatrix  = rotate(mat4(1.0), time, vec3(0, 1.0, 0));
    mat4 mvp          = projectionMatrix * viewMatrix * modelMatrix;
    glUniformMatrix4fv(m_matrixId, 1, GL_FALSE, &mvp[0][0]);

    m_object.Draw();

    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

