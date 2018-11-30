//
// Copyright (C) 2018, Dam Backer
//

#pragma once

class Heatmap
{
public:
	Heatmap();
	~Heatmap();

public:
	void Init();
	void Add(unsigned int x, unsigned int y);

public:
	unsigned int	m_width;
	unsigned int	m_height;
	unsigned int	m_max;
	unsigned int	m_total;
	unsigned int**	m_pData;
};
