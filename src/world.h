#pragma once
#include <vector>

struct Ball;
struct Effector;
struct Joint;
struct World
{
	std::vector<Ball*> balls;
	std::vector<Joint*> joints;
	std::vector<Effector*> effectors;
	float time;
	void update(float d, unsigned int iters = 1);
};
