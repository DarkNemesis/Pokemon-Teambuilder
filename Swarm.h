#pragma once
#include "Particle.h"

class Swarm
{
	vector<Particle> mParticles;

public:
	Swarm(vector<int> Head);

	void Update();
	vector<vector<int>> GetTopRules(unsigned int count);

public:
	static default_random_engine mRNGEngine;
	static uniform_int_distribution<unsigned int> mRNGRange;
	static unsigned int mSize;
};