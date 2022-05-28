#pragma once
#include <vector>

struct Ball;
struct Effector;
struct World
{
	std::vector<Ball*> balls;
	std::vector<Effector*> effectors;
	void update(float d, unsigned int iters = 1);
};
