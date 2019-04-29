#include "ParticleSwarm.h"

vector<array<int, 6>> ParticleSwarm::mTeams;

vector<vector<int>> ParticleSwarm::getTopRules(vector<int> head, unsigned int count)
{
	Swarm mSwarm(head);
	
	for (int i = 0; i < 100; i++)
		mSwarm.Update();

	return mSwarm.GetTopRules(count);
}
