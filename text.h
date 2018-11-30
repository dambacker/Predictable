//
// Copyright (C) 2018, Dam Backer
//

#pragma once

#include "object.h"

typedef struct
{
	unsigned int	x;
	unsigned int	y;
	unsigned int	w;
	unsigned int	h;
	unsigned int	cw;
	unsigned int	ch;
	unsigned int	width;
	unsigned int	height;
	unsigned int	rows;
	unsigned int	cols;
	unsigned int	first;
} Font;

typedef struct
{
	GLfloat	x;
	GLfloat	y;
	GLfloat	u;
	GLfloat	v;
} TextVertex;

class Text
{
public:
	Text();
	~Text();

public:
	int  Init(unsigned int rows, unsigned int cols);
	void Render(unsigned int x, unsigned int y, const char* str);

private:
	GLuint			m_vertexBuffer;
	GLuint			m_programId;
	GLuint			m_textureId;
	GLuint			m_texture;
	Font			m_font;
	unsigned int	m_rows;
	unsigned int	m_cols;
	TextVertex*		m_vertexData;
};

