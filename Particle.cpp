#include "Particle.h"
#include "ParticleSwarm.h"

#include <chrono>

default_random_engine Particle::mRNGEngine(std::chrono::system_clock::now().time_since_epoch().count());
uniform_real_distribution<double> Particle::mRNGRange(0.0f, 1.0f);

#define RNG (float)mRNGRange(mRNGEngine)

#define NeighborhoodSize 10

Particle::Particle(vector<int> head, vector<int> tail) : mVelocity(tail.size(), 0.0f)
{
	mHead = head;
	mTail = tail;

	update();

	mLocalBestC = new Particle();
	mLocalBestS = new Particle();

	mParticleBest = new Particle();
	*mParticleBest = *this;
}

Particle::Particle()
{
}

Particle::~Particle()
{
	//if (mLocalBestC)
	//	delete mLocalBestC;

	//if (mLocalBestS)
	//	delete mLocalBestS;

	//if (mParticleBest)
	//	delete mParticleBest;
}


tCount Particle::findCount()
{
	long long unsigned int mWithHeadCount = 0;
	long long unsigned int mWithHeadAndTailCount = 0;

	for (int i = 0; i < ParticleSwarm::mTeams.size(); i++)
	{
		//First check for Head
		{
			if (ParticleSwarm::mTeams[i][5] < mHead.front())
				continue;

			if (ParticleSwarm::mTeams[i][0] > mHead.back())
				break;

			int k = 0;
			bool flag = true;
			for (int j = 0; j < mHead.size(); j++)
			{
				while (ParticleSwarm::mTeams[i][k] < mHead[j])
					k++;
				if (ParticleSwarm::mTeams[i][k] > mHead[j])
				{
					flag = false;
					break;
				}
				if (ParticleSwarm::mTeams[i][k] == mHead[j])
				{
					k++;
					continue;
				}
			}
			if (flag == true)
			{
				mWithHeadCount++;
			}
		}

		{
			if (ParticleSwarm::mTeams[i][5] < mTail.front())
				continue;

			if (ParticleSwarm::mTeams[i][0] > mTail.back())
				break;

			int k = 0;
			bool flag = true;
			for (int j = 0; j < mTail.size(); j++)
			{
				while (ParticleSwarm::mTeams[i][k] < mTail[j])
					k++;
				if (ParticleSwarm::mTeams[i][k] > mTail[j])
				{
					flag = false;
					break;
				}
				if (ParticleSwarm::mTeams[i][k] == mTail[j])
				{
					k++;
					continue;
				}
			}
			if (flag == true)
			{
				mWithHeadAndTailCount++;
			}
		}
	}
	return make_pair(mWithHeadCount, mWithHeadAndTailCount);
}
float Particle::findSupport(tCount evidence)
{
	return (float)evidence.second / (float)ParticleSwarm::mTeams.size();
}
float Particle::findConfidence(tCount evidence)
{
	return (float)evidence.second / (float)evidence.first;
}
float Particle::findQuality(tCount evidence)
{
	return findSupport(evidence) + findConfidence(evidence);
}

void Particle::update()
{
	auto evidence = findCount();

	mSupport = findSupport(evidence);
	mConfidence = findConfidence(evidence);
	mQuality = findQuality(evidence);

	mPositon = 0;
	for (auto itr : mTail)
		mPositon += itr;
}

bool Particle::sortPosition(Particle a, Particle b)
{
	return a.mPositon < b.mPositon;
}

bool Particle::sortSupport(Particle a, Particle b)
{
	return a.mSupport < b.mSupport;
}

bool Particle::sortConfidence(Particle a, Particle b)
{
	return a.mConfidence < b.mConfidence;
}

bool Particle::sortQuality(Particle a, Particle b)
{
	return a.mQuality < b.mQuality;
}

void Particle::findLocalBestS(vector<Particle>& mParticles, int index)
{	
	if (index + NeighborhoodSize >= mParticles.size())
	{
		list<Particle> neighbors(mParticles.begin() + index, mParticles.end());
		
		neighbors.sort(sortPosition);

		*mLocalBestS = neighbors.front();
		for (auto itr : neighbors)
		{
			if (itr.mConfidence > mLocalBestS->mConfidence)
				*mLocalBestS = itr;
		}
	}
	else
	{
		list<Particle> neighbors(mParticles.begin() + index, mParticles.begin() + index + NeighborhoodSize);

		neighbors.sort(sortPosition);

		for (auto itr = mParticles.begin() + index + NeighborhoodSize; itr != mParticles.end(); itr++)
		{
			if ((*itr).mPositon >= neighbors.back().mPositon)
				continue;
			for (auto jtr = neighbors.begin(); jtr != neighbors.end(); jtr++)
			{
				if ((*itr).mPositon < (*jtr).mPositon)
				{
					neighbors.insert(jtr, *itr);
					neighbors.pop_back();
					break;
				}
			}
		}
		*mLocalBestS = neighbors.front();
		for (auto itr : neighbors)
		{
			if (itr.mConfidence > mLocalBestS->mConfidence)
				*mLocalBestS = itr;
		}
	}		
}

void Particle::findLocalBestC(vector<Particle>& mParticles, int index)
{
	if (index + NeighborhoodSize >= mParticles.size())
	{
		list<Particle> neighbors(mParticles.begin() + index, mParticles.end());

		neighbors.sort(sortPosition);

		*mLocalBestC = neighbors.front();
		for (auto itr : neighbors)
		{
			if (itr.mSupport > mLocalBestC->mSupport)
				*mLocalBestC = itr;
		}
	}
	else
	{
		list<Particle> neighbors(mParticles.begin() + index, mParticles.begin() + index + NeighborhoodSize);

		neighbors.sort(sortPosition);

		for (auto itr = mParticles.begin() + index + NeighborhoodSize; itr != mParticles.end(); itr++)
		{
			if ((*itr).mPositon >= neighbors.back().mPositon)
				continue;
			for (auto jtr = neighbors.begin(); jtr != neighbors.end(); jtr++)
			{
				if ((*itr).mPositon < (*jtr).mPositon)
				{
					neighbors.insert(jtr, *itr);
					neighbors.pop_back();
					break;
				}
			}
		}
		*mLocalBestC = neighbors.front();
		for (auto itr : neighbors)
		{
			if (itr.mSupport > mLocalBestC->mSupport)
				*mLocalBestC = itr;
		}
	}
}

void Particle::findParticleBest()
{
	if (mQuality > mParticleBest->mQuality)
		*mParticleBest = *this;
}

vector<float> Particle::operator+(Particle b)
{
	vector<float> p;
	for (int i = 0; i < mTail.size(); i++)
	{
		p.push_back(mTail[i] + b.mTail[i]);
	}
	return p;
}

vector<float> Particle::operator+(vector<float> b)
{
	vector<float> p;
	for (int i = 0; i < mTail.size(); i++)
	{
		p.push_back(mTail[i] + b[i]);
	}
	return p;
}

vector<float> Particle::operator-(Particle b)
{
	vector<float> p;
	for (int i = 0; i < mTail.size(); i++)
	{
		p.push_back(mTail[i] - b.mTail[i]);
	}
	return p;
}

vector<float> Particle::operator-(vector<float> b)
{
	vector<float> p;
	for (int i = 0; i < mTail.size(); i++)
	{
		p.push_back(mTail[i] - b[i]);
	}
	return p;
}

vector<float> Particle::operator*(float b)
{
	vector<float> p;
	for (int i = 0; i < mTail.size(); i++)
	{
		p.push_back(mTail[i] * b);
	}
	return p;
}

void Particle::computeVelocity()
{
	auto a1 = ((*mLocalBestS - *this) * RNG);
	auto a2 = ((*mLocalBestC - *this) * RNG);
	auto a3 = ((*mParticleBest - *this) * RNG);

	auto a0 = (0.5f + RNG / 2.0f) * mVelocity;

	mVelocity = a0 + 1.49445 * (a1 + a2 + a3);
}

void Particle::move()
{
	for (int i = 0; i < mTail.size(); i++)
	{
		mTail[i] = mTail[i] + std::round(mVelocity[i]);
	}
}

vector<int> Particle::getTail()
{
	return mTail;
}

vector<float> operator*(vector<float> a, float b)
{
	vector<float> p;
	for (int i = 0; i < a.size(); i++)
	{
		p.push_back(a[i] * b);
	}
	return p;
}

vector<float> operator*(float b, vector<float> a)
{
	vector<float> p;
	for (int i = 0; i < a.size(); i++)
	{
		p.push_back(a[i] * b);
	}
	return p;
}

vector<float> operator+(vector<float> a, vector<float> b)
{
	vector<float> p;
	for (int i = 0; i < a.size(); i++)
	{
		p.push_back(a[i] * b[i]);
	}
	return p;
}

vector<float> operator-(vector<float> a, vector<float> b)
{
	vector<float> p;
	for (int i = 0; i < a.size(); i++)
	{
		p.push_back(a[i] - b[i]);
	}
	return p;
}