#pragma once

struct Joint
{
	virtual void apply(float delta) = 0;
};

struct Ball;
struct SpringJoint : Joint
{
	Ball* a, *b;
	float target_length, stiffness;

	SpringJoint(Ball* a_, Ball* b_, float tl_ = 5, float stiffness_ = 1) :
		a(a_), b(b_), target_length(tl_), stiffness(stiffness_) {}

	void apply(float delta) override;
};
