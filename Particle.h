#pragma once
#include <vector>
#include <array>
#include <list>
#include <random>

using namespace std;

typedef pair<long long unsigned int, long long unsigned int> tCount;

class Particle
{
	vector<int> mHead;
	vector<int> mTail;
	vector<float> mVelocity;

	float mSupport;
	float mConfidence;
	float mQuality;
	float mPositon;

	Particle*	mLocalBestS = nullptr;
	Particle*	mLocalBestC = nullptr;
	Particle*	mParticleBest = nullptr;

	float findConfidence(tCount evidence);
	float findQuality(tCount evidence);
	float findSupport(tCount evidence);

	tCount findCount();
	Particle();

public:
	Particle(vector<int> head, vector<int> tail);	
	~Particle();

	static bool sortPosition(Particle a, Particle b);
	static bool sortSupport(Particle a, Particle b);
	static bool sortConfidence(Particle a, Particle b);
	static bool sortQuality(Particle a, Particle b);

	static default_random_engine mRNGEngine;
	static uniform_real_distribution<double> mRNGRange;

	void findLocalBestS(vector<Particle>& mParticles, int index);
	void findLocalBestC(vector<Particle>& mParticles, int index);
	void findParticleBest();

	vector<float> operator+ (Particle b);
	vector<float> operator+	(vector<float> b);

	vector<float> operator- (Particle b);
	vector<float> operator-	(vector<float> b);

	vector<float> operator* (float b);

	void computeVelocity();
	void move();

	vector<int> getTail();

	void update();
};

vector<float> operator* (vector<float> a, float b);
vector<float> operator* (float b, vector<float> a);

vector<float> operator+ (vector<float> a, vector<float> b);
vector<float> operator- (vector<float> a, vector<float> b);