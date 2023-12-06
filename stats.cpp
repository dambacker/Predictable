//
// Copyright (C) 2018-2019, Dam Backer
//

#include "pch.h"

#include <math.h>

#include "stats.h"

float Sqrf(float x)
{
	return (x*x);
}

//-----------------------------------------------------------------------------
Stats::Stats()
//-----------------------------------------------------------------------------
{
	m_count				= 0;
	m_sumTotal			= 0.0f;
	m_sumTotalSquare	= 0.0f;
}

//-----------------------------------------------------------------------------
Stats::~Stats()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Stats::Add(float x)
//-----------------------------------------------------------------------------
{
	m_sumTotal			+= x;
	m_sumTotalSquare	+= Sqrf(x);

	m_count++;
}

//-----------------------------------------------------------------------------
float Stats::Average()
//-----------------------------------------------------------------------------
{
	if (m_count < 1)
		return 0.0f;

	return m_sumTotal / m_count;
}

//-----------------------------------------------------------------------------
float Stats::StandardDeviation()
//-----------------------------------------------------------------------------
{
	if (m_count < 1)
		return 0.0f;

	// Standard deviation approximation
	// https://en.wikipedia.org/wiki/Standard_deviation#Identities_and_mathematical_properties
	//
	return sqrtf(m_sumTotalSquare/m_count - Sqrf(m_sumTotal/m_count));
}

