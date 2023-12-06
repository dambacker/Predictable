//
// Copyright (C) 2018, Dam Backer
//

#include "pch.h"

#include <windows.h>
#include <fstream>
#include <sstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "utils.h"
#include "object.h"
#include "texture.h"
#include "scene01.h"
#include "scene02.h"
#include "scene03.h"
#include "app.h"

//-----------------------------------------------------------------------------
App::App()
//-----------------------------------------------------------------------------
{
    m_pScene01 = NULL;
    m_pScene02 = NULL;
    m_pScene03 = NULL;
}

//-----------------------------------------------------------------------------
App::~App()
//-----------------------------------------------------------------------------
{
    delete(m_pScene01);
    delete(m_pScene02);
    delete(m_pScene03);
}

//-----------------------------------------------------------------------------
unsigned int App::Init(int width, int height)
//-----------------------------------------------------------------------------
{
    Scale(width, height);

    m_mouse.Init();
    m_text.Init(80, 50);

    //heatmap
    m_heatmap.Init();
    glGenTextures(1, &m_textureHeatmap);
    m_heatmapTextureData = new unsigned char[m_heatmap.m_width*m_heatmap.m_height * 3];

    //scene01
    m_pScene01 = new(Scene01);
    if (m_pScene01->Init(m_aspectX, m_aspectY) != R_OK)
    {
        return R_ERROR;
    }

    //scene02
    m_pScene02 = new(Scene02);
    if (m_pScene02->Init() != R_OK)
    {
        return R_ERROR;
    }

    //scene03
    m_pScene03 = new(Scene03);
    if (m_pScene03->Init() != R_OK)
    {
        return R_ERROR;
    }

    // Set default GL state
    glGenVertexArrays(1, &m_vertexArrayId); //not sure why this is required
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
void App::Scale(int width, int height)
//-----------------------------------------------------------------------------
{
    m_width  = width;
    m_height = height;

    //set aspect ratio such that [-1.0..1.0] is visible
    if (width > height)
    {
        m_aspectX = (float)height/(float)width;
        m_aspectY = 1.0f;
    }
    else
    {
        m_aspectX = 1.0f;
        m_aspectY = (float)width/(float)height;
    }
}

//-----------------------------------------------------------------------------
void App::Update(float xpos, float ypos)
//-----------------------------------------------------------------------------
{
    unsigned int x = (unsigned int)(xpos * m_heatmap.m_width);
    unsigned int y = (unsigned int)(ypos * m_heatmap.m_height);

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
void App::Mouse(float mouseX, float mouseY, int mouseButton1)
//-----------------------------------------------------------------------------
{
    m_mouseX        = mouseX;
    m_mouseY        = mouseY;
    m_mouseButton1  = mouseButton1;

    if (m_mouseButton1 == GLFW_PRESS)
    {
        Update(m_mouseX, m_mouseY);
    }
}

//-----------------------------------------------------------------------------
void App::Render(float time)
//-----------------------------------------------------------------------------
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    m_pScene01->Render(time);
//    m_pScene02->Render(time);
    m_pScene03->Render(time, m_textureHeatmap);

    m_text.Render(1, 1, "Predictable");

    m_mouse.Render(2.0f*m_mouseX-1.0f, 2.0f*m_mouseY-1.0f);
}

