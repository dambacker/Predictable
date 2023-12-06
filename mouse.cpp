//
// Copyright (C) 2018, Dam Backer
//

#include "pch.h"

#include <windows.h>
#include <fstream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "utils.h"
#include "object.h"
#include "mouse.h"

//-----------------------------------------------------------------------------
CMouse::CMouse()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
CMouse::~CMouse()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
int CMouse::Init()
//-----------------------------------------------------------------------------
{
	m_programId = LoadShaders("shaders/mouse.vs", "shaders/mouse.fs");
	if (m_programId == 0)
	{
		printf("Error loading text shaders");
		return 0;
	}
	m_matrixId  = glGetUniformLocation(m_programId, "modelViewProjection");
	m_textureId = glGetUniformLocation(m_programId, "textureSampler");

	m_texture = LoadTexture("textures/mouse.bmp");
	if (m_texture == 0)
	{
		printf("Error loading mouse texture");
		return 0;
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	static const GLfloat vertexBufferData[] =
	{
		 1.0f, 1.0f,   1.0f, 1.0f,
		-1.0f, 1.0f,   0.0f, 1.0f,
		-1.0f,-1.0f,   0.0f, 0.0f,

		 1.0f, 1.0f,   1.0f, 1.0f,
		-1.0f,-1.0f,   0.0f, 0.0f,
		 1.0f,-1.0f,   1.0f, 0.0f,
	};

	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 4*6*sizeof(float), vertexBufferData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return 1;
}

//-----------------------------------------------------------------------------
void CMouse::Render(float x, float y)
//-----------------------------------------------------------------------------
{
	glUseProgram(m_programId);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glUniform1i(m_textureId, 0);

	float scale = 0.05f;		//size of cursor

	float mvp[4][4] = 
	{
		scale, 0.0f,  0.0f, 0.0f,
		0.0f,  scale, 0.0f, 0.0f,
		0.0f,  0.0f,  0.0f, 0.0f,
		x,     y,     0.0f, 1.0f,
	};
	glUniformMatrix4fv(m_matrixId, 1, GL_FALSE, &mvp[0][0]);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)6);

	glDisableVertexAttribArray(0);
}

