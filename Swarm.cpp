#include "Swarm.h"

#include <chrono>

default_random_engine Swarm::mRNGEngine(std::chrono::system_clock::now().time_since_epoch().count());
uniform_int_distribution<unsigned int> Swarm::mRNGRange(1, 801);

unsigned int Swarm::mSize = 250;

#define RNG (int)mRNGRange(mRNGEngine)

Swarm::Swarm(vector<int> head)
{
	for (int i = 0; i < mSize; i++)
	{
		vector<int> tail;
		for (int j = 0; j < 6 - head.size(); j++)
		{
			int c;
			do
			{
				c = RNG;
			} while (find(head.begin(), head.end(), c) != head.end());
			tail.push_back(c);
		}
				
		mParticles.push_back(Particle(head, tail));
	}
}

void Swarm::Update()
{
	//using namespace std::chrono;

	//milliseconds ms1 = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	sort(mParticles.begin(), mParticles.end(), Particle::sortSupport);
	for (int i = 0; i < mParticles.size(); i++)
		mParticles[i].findLocalBestS(mParticles, i);

	//milliseconds ms2 = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	//auto d = ms2 - ms1;

	//milliseconds ms3 = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	
	sort(mParticles.begin(), mParticles.end(), Particle::sortConfidence);
	for (int i = 0; i < mParticles.size(); i++)
		mParticles[i].findLocalBestC(mParticles, i);

	//milliseconds ms4 = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	//auto d2 = ms4 - ms3;

	for (int i = 0; i < mParticles.size(); i++)
		mParticles[i].findParticleBest();

	for (auto& itr : mParticles)
		itr.computeVelocity();

	for (auto& itr : mParticles)
		itr.move();

	for (auto& itr : mParticles)
		itr.update();
}

vector<vector<int>> Swarm::GetTopRules(unsigned int count)
{
	vector<vector<int>> result;

	sort(mParticles.begin(), mParticles.end(), Particle::sortQuality);
	for (int i = 0; i < count; i++)
		result.push_back(mParticles[i].getTail());

	return result;
}
