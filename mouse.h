//
// Copyright (C) 2018, Dam Backer
//

#pragma once

class CMouse
{
public:
	CMouse();
	~CMouse();

public:
	int  Init();
	void Render(float x, float y);

private:
	GLuint			m_vertexBuffer;
	GLuint			m_programId;
	GLuint			m_matrixId;
	GLuint			m_textureId;
	GLuint			m_texture;
};

