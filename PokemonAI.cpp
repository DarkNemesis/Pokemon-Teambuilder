#include <iostream>
#include "ParticleSwarm.h"

int main()
{
	//Read the file first
	ParticleSwarm::ReadFile("teaminfo1.csv");
	//Store it in Swarm
	vector<int> core{465, 658};
	auto test = ParticleSwarm::getTopRules(core, 10);
	for (auto itr : test)
	{
		for (auto jtr : itr)
			cout << jtr << " ";

		cout << endl;
	}
}
