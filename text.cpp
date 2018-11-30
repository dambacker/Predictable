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
#include "text.h"

//-----------------------------------------------------------------------------
Text::Text()
//-----------------------------------------------------------------------------
{
	m_vertexData = NULL;
}

//-----------------------------------------------------------------------------
Text::~Text()
//-----------------------------------------------------------------------------
{
	if (m_vertexData != NULL)
	{
		free(m_vertexData);
		m_vertexData = NULL;
	}
}

//-----------------------------------------------------------------------------
int Text::Init(unsigned int rows, unsigned int cols)
//-----------------------------------------------------------------------------
{
	m_programId = LoadShaders("shaders/text.vs", "shaders/text.fs");
	if (m_programId == 0)
	{
		printf("Error loading text shaders");
		return 0;
	}
	m_textureId = glGetUniformLocation(m_programId, "textureSampler");

	m_texture = LoadTexture("textures/font02.bmp");
	if (m_texture == 0)
	{
		printf("Error loading text font");
		return 0;
	}
	//magic numbers specific to the font texture
	m_font.x		= 10;
	m_font.y		= 10;
	m_font.w		= 40;
	m_font.h		= 40;
	m_font.cw		= 72;
	m_font.ch		= 60;
	m_font.width	= 720;
	m_font.height	= 600;
	m_font.rows		= 10;
	m_font.cols		= 10;
	m_font.first	= 22;

	m_rows = rows;
	m_cols = cols;

	m_vertexData = (TextVertex*)malloc(sizeof(TextVertex) * 6 * m_rows);	//max stringlength should always be truncated to rows

	glGenBuffers(1, &m_vertexBuffer);

	return 1;
}

//-----------------------------------------------------------------------------
void Text::Render(unsigned int x, unsigned int y, const char* str)
//-----------------------------------------------------------------------------
{
	size_t length = strlen(str);

	if ((length == 0) || (x > m_rows) || (y > m_cols))
	{
		return;
	}
	if (x + length > m_rows)
	{
		length = m_rows - x;
	}

	for (int i = 0; i < length; i++)
	{
		unsigned char c = str[i] - m_font.first;

		unsigned int xi = c % m_font.rows;
		unsigned int yi = m_font.cols - c / m_font.cols;

		for (int v = 0; v < 6; v++)
		{
			unsigned int vx = (v==1 || v==2 || v==5)?0:1;
			unsigned int vy = (v==1 || v==4 || v==5)?0:1;

			m_vertexData[i*6+v].x = (x + i + vx) * 2.0f / m_rows - 1.0f;
			m_vertexData[i*6+v].y = 1.0f - (y     + vy) * 2.0f / m_cols;
			m_vertexData[i*6+v].u = (xi+0.34f+vx*0.30f)/(float)m_font.rows;
			m_vertexData[i*6+v].v = (yi+0.18f+(1-vy)*0.40f)/(float)m_font.cols;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TextVertex) * 6 * length, m_vertexData, GL_STATIC_DRAW);

	glUseProgram(m_programId);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glUniform1i(m_textureId, 0);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (void*)0);
	
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (void*)(sizeof(float)*2));

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)length*6);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

