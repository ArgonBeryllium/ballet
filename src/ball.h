#pragma once
#include "v2f.h"

struct Ball
{
	v2f pos, p_pos;
	v2f acc;
	float r;
	float mass;

	Ball(const v2f& pos_ = {0,0}, float r_ = 1, float mass_ = 1, const v2f& iv = {0,0}) :
		pos(pos_), p_pos(pos-iv), r(r_), mass(mass_) {}

	void update(float d);
	void accelerate(const v2f& a);
	void teleport(const v2f& np);
};
