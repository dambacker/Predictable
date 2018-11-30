//
// Copyright (C) 2018, Dam Backer
//

#pragma once

#define MAX_ATTRIBUTES	8

enum
{
	ATTRIB_VERTEX	= 0x00000001,
	ATTRIB_TEXCOORD = 0x00000002,
};

typedef struct
{
	GLuint		attrib;
	GLuint		size;
	GLuint		type;
} AttribInfo;

class Object
{
public:
	Object();
	~Object();

public:
	void		CreateQuad();
	void		CreateCube(int divs);
	void		CreateCube2(int divs);
	void		CreateDonut();
	void		Draw();

protected:
	GLuint		m_vertexBuffer;
	GLuint		m_vertexSize;
	GLuint		m_texcoordSize;
	GLuint		m_vertexCount;
	GLuint		m_elementBuffer;
	GLshort		m_indexCount;

	GLuint		m_numAttributes;
	GLuint		m_attributeList;
	AttribInfo	m_attribInfo[MAX_ATTRIBUTES];
};
