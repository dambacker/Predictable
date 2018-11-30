//
// Copyright (C) 2018, Dam Backer
//

#include "pch.h"

#include <windows.h>

#include "heatmap.h"

//-----------------------------------------------------------------------------
Heatmap::Heatmap()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Heatmap::~Heatmap()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Heatmap::Init()
//-----------------------------------------------------------------------------
{
	m_width		= 16;
	m_height	= 16;
	m_max		= 0;
	m_total		= 0;

	m_pData = (unsigned int**)malloc(m_width * sizeof(unsigned int*));
	for (unsigned int x = 0; x < m_width; x++)
	{
		m_pData[x] = (unsigned int*)malloc(m_height * sizeof(unsigned int));
		for (unsigned int y = 0; y < m_height; y++)
		{
			m_pData[x][y] = 0;
		}
	}
}

//-----------------------------------------------------------------------------
void Heatmap::Add(unsigned int x, unsigned int y)
//-----------------------------------------------------------------------------
{
	m_pData[x][y]++;
	m_total++;
	if (m_pData[x][y] > m_max)
	{
		m_max = m_pData[x][y];
	}
}