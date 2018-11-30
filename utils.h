//
// Copyright (C) 2018, Dam Backer
//

#pragma once

void*	SafeMalloc(size_t size);
void	_SafeFree(void* ptr);
#define SafeFree(ptr)	{_SafeFree((void*)(ptr));(ptr)=NULL;}

int		LoadFileToString(const char* pFilePath, std::string &str);
int		LoadFileToMemory(char const* pFilePath, unsigned char** ppData);

GLuint	LoadShaders(const char* pVertexFilePath, const char* pFragmentFilePath);
GLuint	LoadTexture(char const* pFilePath);

