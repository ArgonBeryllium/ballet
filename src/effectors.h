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
struct EffSpring : Effector
{
	Ball* a, *b;
	float target_length, force;

	EffSpring(Ball* a_, Ball* b_, float tl_ = 5, float force_ = 1) :
		a(a_), b(b_), target_length(tl_), force(force_) {}

	void apply(std::vector<Ball*>& balls, float d) override;
};
