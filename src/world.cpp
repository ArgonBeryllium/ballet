#include "world.h"
#include "ball.h"
#include "effectors.h"
void World::update(float d, unsigned int iters)
{
	d /= iters;
	for(unsigned int i = 0; i != iters; i++)
	{
		for(auto b : balls)
			b->update(d);
		for(auto e : effectors)
			e->apply(balls, d);
	}
}
