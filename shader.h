//
// Copyright (C) 2018, Dam Backer
//

#pragma once

class CShader
{
public:
	CShader();
	~CShader();

public:
	GLuint	LoadFromFile(const char* pVertexFilePath, const char* pFragmentFilePath);

public:
	GLuint	m_program;
};

