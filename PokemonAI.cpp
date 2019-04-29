#include <iostream>
#include "ParticleSwarm.h"

int main()
{
	//Read the file first
	//Store it in Swarm
	vector<int> core{12, 45};
	auto test = ParticleSwarm::getTopRules(core, 10);
	for (auto itr : test)
	{
		for (auto jtr : itr)
			cout << jtr << " ";

		cout << endl;
	}
}
