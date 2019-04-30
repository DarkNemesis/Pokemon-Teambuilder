#include "ParticleSwarm.h"
#include <fstream>
#include <sstream>

vector<array<int, 6>> ParticleSwarm::mTeams;

vector<vector<int>> ParticleSwarm::getTopRules(vector<int> head, unsigned int count)
{
	Swarm mSwarm(head);
	
	for (int i = 0; i < 100; i++)
		mSwarm.Update();

	return mSwarm.GetTopRules(count);
}

void ParticleSwarm::ReadFile(const char * path)
{
	ifstream inFile;
	inFile.open(path, ifstream::in);
	while (inFile)
	{
		string team;
		if (!getline(inFile, team)) break;

		istringstream ss(team);
		array<int, 6> teamIndex;
		for(int i = 0; i < 6; i++)
		{
			string s;
			if (!getline(ss, s, ',')) break;

			teamIndex[i] = std::atoi(s.c_str());
		}
		mTeams.push_back(teamIndex);
	}
}
