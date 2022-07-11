#pragma once

struct Ball;
struct Joint
{
	Ball* a, *b;
	Joint(Ball* a_, Ball* b_) : a(a_), b(b_) {}
	virtual void apply(float delta) = 0;
};

struct SpringJoint : Joint
{
	float target_length, stiffness;

	SpringJoint(Ball* a_, Ball* b_, float tl_ = 5, float stiffness_ = 1) :
		Joint(a_, b_), target_length(tl_), stiffness(stiffness_) {}

	void apply(float delta) override;
};
struct StiffJoint : Joint
{
	float target_length;
	StiffJoint(Ball* a_, Ball* b_, float tl_ = 5) :
		Joint(a_, b_), target_length(tl_) {}

	void apply(float delta) override;
};
