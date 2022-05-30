#pragma once
#include <vector>
#include "v2f.h"

struct Ball;
struct Effector
{
	virtual void apply(std::vector<Ball*>& balls, float d) = 0;
};
struct EffGravity : Effector
{
	v2f g = {0, 16000};

	void apply(std::vector<Ball*>& balls, float d) override;
};
struct EffCollision : Effector
{
	void apply(std::vector<Ball*>& balls, float d) override;
};
