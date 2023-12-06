//
// Copyright (C) 2018-2019, Dam Backer
//

#pragma once

class Stats
{
public:
	Stats();
	~Stats();

public:
	void	Add(float x);
	float	Average();
	float	StandardDeviation();

public:
	unsigned int	m_count;
	float			m_sumTotalSquare;
	float			m_sumTotal;
};
