//
// Copyright (C) 2018, Dam Backer
//

#include "pch.h"

#include <windows.h>
#include <fstream>
#include <sstream>
#include <vector>

#include <GL/glew.h>

#include "object.h"

//-----------------------------------------------------------------------------
Object::Object()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Object::~Object()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Object::CreateQuad()
//-----------------------------------------------------------------------------
{
	static const GLfloat vertexBufferData[] =
	{
		  1.0f, 1.0f,	   1.0f, 1.0f,
		 -1.0f, 1.0f,	   0.0f, 1.0f,
		 -1.0f,-1.0f,	   0.0f, 0.0f,

		  1.0f, 1.0f,	   1.0f, 1.0f,
		 -1.0f,-1.0f,	   0.0f, 0.0f,
		  1.0f,-1.0f,	   1.0f, 0.0f,
	};

	m_vertexSize = 2;
	m_texcoordSize = 2;
	m_vertexCount = 6;

	static const GLshort indexBufferData[] =
	{
		 0,  1,  2,  3,  4,  5
	};
	m_indexCount = 6;

	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, (m_vertexSize + 2) * m_vertexCount * sizeof(float), vertexBufferData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(GLshort), indexBufferData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//-----------------------------------------------------------------------------
void Object::CreateCube(int divs)
//-----------------------------------------------------------------------------
{
	static const GLfloat vertexBufferData[] =
	{
		//-x
		-1.0f,-1.0f,-1.0f,     0.0f, 0.0f,
		-1.0f,-1.0f, 1.0f,	   0.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,	   1.0f, 1.0f,

		-1.0f,-1.0f,-1.0f,	   0.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,	   1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,	   1.0f, 0.0f,
		//+x
		 1.0f, 1.0f, 1.0f,	   1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,	   0.0f, 0.0f,
		 1.0f, 1.0f,-1.0f,	   1.0f, 0.0f,

		 1.0f,-1.0f,-1.0f,	   0.0f, 0.0f,
		 1.0f, 1.0f, 1.0f,	   1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,	   0.0f, 1.0f,
		//-y
		 1.0f,-1.0f, 1.0f,	   1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,	   0.0f, 0.0f,
		 1.0f,-1.0f,-1.0f,	   1.0f, 0.0f,

		 1.0f,-1.0f, 1.0f,	   1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,	   0.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,	   0.0f, 0.0f,
		//+y				  
		 1.0f, 1.0f, 1.0f,	   1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,	   1.0f, 0.0f,
		-1.0f, 1.0f,-1.0f,	   0.0f, 0.0f,

		 1.0f, 1.0f, 1.0f,	   1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,	   0.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,	   0.0f, 1.0f,
		//-z
		 1.0f, 1.0f,-1.0f,	   1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,	   0.0f, 0.0f,
		-1.0f, 1.0f,-1.0f,	   0.0f, 1.0f,

		 1.0f, 1.0f,-1.0f,	   1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,	   1.0f, 0.0f,
		-1.0f,-1.0f,-1.0f,	   0.0f, 0.0f,
		//+z				  
		-1.0f, 1.0f, 1.0f,	   0.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,	   0.0f, 0.0f,
		 1.0f,-1.0f, 1.0f,	   1.0f, 0.0f,

		 1.0f, 1.0f, 1.0f,	   1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,	   0.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,	   1.0f, 0.0f,
	};

	m_vertexSize = 3;
	m_texcoordSize = 2;
	m_vertexCount = 36;

	static const GLshort indexBufferData[] =
	{
		 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
		10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
		20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35
	};
	m_indexCount = 36;

	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, (m_vertexSize + 2) * m_vertexCount * sizeof(float), vertexBufferData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(GLshort), indexBufferData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//-----------------------------------------------------------------------------
void Object::CreateCube2(int divs)
//-----------------------------------------------------------------------------
{
	m_vertexCount = 36;

	m_numAttributes = 2;

	//TODO: change all objects to use generic attribInfo instead of m_vertexSize + m_texcoordSize
	m_attribInfo[0].attrib	= ATTRIB_VERTEX;
	m_attribInfo[0].size	= 3;
	m_attribInfo[0].type	= GL_FLOAT;

	m_attribInfo[1].attrib	= ATTRIB_TEXCOORD;
	m_attribInfo[1].size	= 2;
	m_attribInfo[1].type	= GL_FLOAT;

	static const GLfloat vertexBufferData[] =
	{
		//-x
		-1.0f,-1.0f,-1.0f,     1.0f, 0.0f,
		-1.0f,-1.0f, 1.0f,	   0.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,	   0.0f, 1.0f,

		-1.0f,-1.0f,-1.0f,	   1.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,	   0.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,	   1.0f, 1.0f,
		//+x
		 1.0f, 1.0f, 1.0f,	   1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,	   0.0f, 0.0f,
		 1.0f, 1.0f,-1.0f,	   0.0f, 1.0f,

		 1.0f,-1.0f,-1.0f,	   0.0f, 0.0f,
		 1.0f, 1.0f, 1.0f,	   1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,	   1.0f, 0.0f,
		 //-y
		  1.0f,-1.0f, 1.0f,	   1.0f, 1.0f,
		 -1.0f,-1.0f,-1.0f,	   0.0f, 0.0f,
		  1.0f,-1.0f,-1.0f,	   1.0f, 0.0f,

		  1.0f,-1.0f, 1.0f,	   1.0f, 1.0f,
		 -1.0f,-1.0f, 1.0f,	   0.0f, 1.0f,
		 -1.0f,-1.0f,-1.0f,	   0.0f, 0.0f,
		 //+y				  
		  1.0f, 1.0f, 1.0f,	   1.0f, 1.0f,
		  1.0f, 1.0f,-1.0f,	   1.0f, 0.0f,
		 -1.0f, 1.0f,-1.0f,	   0.0f, 0.0f,

		  1.0f, 1.0f, 1.0f,	   1.0f, 1.0f,
		 -1.0f, 1.0f,-1.0f,	   0.0f, 0.0f,
		 -1.0f, 1.0f, 1.0f,	   0.0f, 1.0f,
		 //-z
		  1.0f, 1.0f,-1.0f,	   1.0f, 1.0f,
		 -1.0f,-1.0f,-1.0f,	   0.0f, 0.0f,
		 -1.0f, 1.0f,-1.0f,	   0.0f, 1.0f,

		  1.0f, 1.0f,-1.0f,	   1.0f, 1.0f,
		  1.0f,-1.0f,-1.0f,	   1.0f, 0.0f,
		 -1.0f,-1.0f,-1.0f,	   0.0f, 0.0f,
		 //+z				  
		 -1.0f, 1.0f, 1.0f,	   1.0f, 1.0f,
		 -1.0f,-1.0f, 1.0f,	   1.0f, 0.0f,
		  1.0f,-1.0f, 1.0f,	   0.0f, 0.0f,

		  1.0f, 1.0f, 1.0f,	   0.0f, 1.0f,
		 -1.0f, 1.0f, 1.0f,	   1.0f, 1.0f,
		  1.0f,-1.0f, 1.0f,	   0.0f, 0.0f,
	};

	m_vertexSize = 3;
	m_texcoordSize = 2;
	m_indexCount = 36;

	static const GLshort indexBufferData[] =
	{
		 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
		10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
		20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35
	};

	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, (m_vertexSize + 2) * m_vertexCount * sizeof(float), vertexBufferData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(GLshort), indexBufferData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//-----------------------------------------------------------------------------
void Object::CreateDonut()
//-----------------------------------------------------------------------------
{
	int numSlices = 16;
	int numParts  = 16;

	float radiusSlice = 3.0f;
	float radiusParts = 0.5f;

	GLfloat* pTriangleData = (GLfloat*)malloc(sizeof(GLfloat) * 1);

	for (int s = 0; s < numSlices; s++)
	{
		for (int p = 0; p < numParts; p++)
		{
			float x = sinf((float)s / float(numParts)) * radiusSlice;
			float y = cosf((float)s / float(numParts)) * radiusSlice;
			float z = 0.0f;
		}
	}

	free(pTriangleData);
}

//-----------------------------------------------------------------------------
void Object::Draw()
//-----------------------------------------------------------------------------
{
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glVertexAttribPointer(0, m_vertexSize,   GL_FLOAT, GL_FALSE, sizeof(float) * (m_vertexSize + m_texcoordSize), (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glVertexAttribPointer(1, m_texcoordSize, GL_FLOAT, GL_FALSE, sizeof(float) * (m_vertexSize + m_texcoordSize), (void*)(sizeof(float)*m_vertexSize));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer);

	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_SHORT, (void*)0);

	glDisableVertexAttribArray(0);
}

