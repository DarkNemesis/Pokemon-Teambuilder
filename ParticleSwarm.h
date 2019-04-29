#pragma once
#include "Swarm.h"

class ParticleSwarm
{
public:
	static vector<array<int, 6>> mTeams;
	
	static vector<vector<int>> getTopRules(vector<int> head, unsigned int count);
};


